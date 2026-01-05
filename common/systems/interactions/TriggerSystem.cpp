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
            math::FRect hitbox = collider->getHitbox(
                transform->getPosition(), transform->getScale(), transform->getRotation());
            _spatialGrid.insert(entityId, hitbox);
        }
    }
}

void TriggerSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

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

        math::FRect triggerHitbox = triggerCollider->getHitbox(
            triggerTransform->getPosition(),
            triggerTransform->getScale(),
            triggerTransform->getRotation());

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
            if (otherCollider && shouldCollide(registry, triggerEntity, *triggerCollider,
                    colliderEntity) &&
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
    math::OrientedRect hitboxA = colliderA.getOrientedHitbox(transformA.getPosition(),
        transformA.getScale(), transformA.getRotation());
    math::OrientedRect hitboxB = colliderB.getOrientedHitbox(transformB.getPosition(),
        transformB.getScale(), transformB.getRotation());

    return hitboxA.intersects(hitboxB);
}

bool TriggerSystem::shouldCollide(
    std::shared_ptr<Registry> registry,
    size_t entityA,
    const ColliderComponent& colliderA,
    size_t entityB
) {
    const TagRegistry& tagRegistry = TagRegistry::getInstance();
    const CollisionRules& collisionRules = CollisionRules::getInstance();
    std::vector<std::string> tagsA = tagRegistry.getTags(registry, entityA);
    std::vector<std::string> tagsB = tagRegistry.getTags(registry, entityB);

    return collisionRules.canCollide(colliderA.getType(), tagsA, tagsB);
}

}
