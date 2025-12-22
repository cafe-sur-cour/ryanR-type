/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerSystem
*/

#include "TriggerSystem.hpp"
#include <memory>
#include <string>
#include <vector>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../CollisionRules/CollisionRules.hpp"
#include "../../constants.hpp"
#include "../SystemNames.hpp"

namespace ecs {

TriggerSystem::TriggerSystem() : _spatialGrid(
    constants::MAX_WIDTH,
    constants::MAX_HEIGHT,
    constants::SPATIAL_GRID_CELL_SIZE,
    constants::SPATIAL_GRID_PADDING
) {
}

void TriggerSystem::buildSpatialGrid(std::shared_ptr<Registry> registry) {
    _spatialGrid.clear();

    auto gameZoneView = registry->view<GameZoneComponent, TransformComponent>();
    for (auto entityId : gameZoneView) {
        auto transform = registry->getComponent<TransformComponent>(entityId);
        if (transform) {
            math::Vector2f gameZonePos = transform->getPosition();
            _spatialGrid.setOffset(gameZonePos.getX(), gameZonePos.getY());
            break;
        }
    }

    auto colliderView = registry->view<TransformComponent, ColliderComponent>();
    for (auto entityId : colliderView) {
        auto transform = registry->getComponent<TransformComponent>(entityId);
        auto colliders = registry->getComponents<ColliderComponent>(entityId);

        if (!transform || colliders.empty())
            continue;

        for (auto& collider : colliders) {
            math::FRect hitbox = collider->getScaledHitbox(
                transform->getPosition(), transform->getScale());
            _spatialGrid.insert(entityId, hitbox);
        }
    }
}

void TriggerSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)deltaTime;

    auto tagRegistry = resourceManager->get<TagRegistry>();

    buildSpatialGrid(registry);

    auto colliderView = registry->view<ColliderComponent>();
    for (auto triggerEntity : colliderView) {
        auto triggerCollider = registry->getComponent<ColliderComponent>(triggerEntity);
        if (!triggerCollider || triggerCollider->getType() != CollisionType::Trigger) {
            continue;
        }

        auto triggerTransform = registry->getComponent<TransformComponent>(triggerEntity);
        if (!triggerTransform)
            continue;

        math::FRect triggerHitbox = triggerCollider->getScaledHitbox(
            triggerTransform->getPosition(), triggerTransform->getScale());

        auto nearbyEntities = _spatialGrid.query(triggerHitbox);

        for (auto colliderEntity : nearbyEntities) {
            if (triggerEntity == colliderEntity)
                continue;

            auto colliderTransform = registry->getComponent<TransformComponent>(
                colliderEntity
            );
            if (!colliderTransform)
                continue;

            auto otherCollider = registry->getComponent<ColliderComponent>(colliderEntity);
            if (otherCollider && shouldCollide(resourceManager, registry, triggerEntity,
                *triggerCollider, colliderEntity) &&
                    checkCollision(*triggerTransform, *triggerCollider,
                    *colliderTransform, *otherCollider)
            ) {
                registry->addComponent<TriggerIntentComponent>(triggerEntity,
                    std::make_shared<TriggerIntentComponent>(triggerEntity, colliderEntity));
            }
        }
    }
}

bool TriggerSystem::checkCollision(
    const TransformComponent& transformA,
    const ColliderComponent& colliderA,
    const TransformComponent& transformB,
    const ColliderComponent& colliderB
) {
    math::FRect hitboxA = colliderA.getScaledHitbox(transformA.getPosition(),
        transformA.getScale());
    math::FRect hitboxB = colliderB.getScaledHitbox(transformB.getPosition(),
        transformB.getScale());

    return hitboxA.intersects(hitboxB);
}

bool TriggerSystem::shouldCollide(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    size_t entityA,
    const ColliderComponent& colliderA,
    size_t entityB
) {
    auto tagRegistry = resourceManager->get<TagRegistry>();
    auto collisionRules = resourceManager->get<CollisionRules>();
    std::vector<std::string> tagsA = tagRegistry->getTags(registry, entityA);
    std::vector<std::string> tagsB = tagRegistry->getTags(registry, entityB);

    return collisionRules->canCollide(colliderA.getType(), tagsA, tagsB);
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::TriggerSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::TRIGGER_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
