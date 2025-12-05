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
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../systems/interactions/TagRegistry.hpp"

namespace ecs {

TriggerSystem::TriggerSystem() {
}

void TriggerSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

    auto colliderView = registry->view<ColliderComponent>();
    for (auto triggerEntity : colliderView) {
        auto triggerCollider = registry->getComponent<ColliderComponent>(triggerEntity);
        if (!triggerCollider || triggerCollider->getType() != CollisionType::Trigger) {
            continue;
        }

        for (auto colliderEntity : colliderView) {
            if (triggerEntity == colliderEntity)
                continue;

            auto triggerTransform = registry->getComponent<
                TransformComponent>(triggerEntity);
            auto colliderTransform = registry->getComponent<
                TransformComponent>(colliderEntity);

            if (!triggerTransform || !colliderTransform)
                continue;

            auto otherCollider = registry->getComponent<ColliderComponent>(colliderEntity);
            if (otherCollider && shouldCollide(registry, triggerEntity, *triggerCollider,
                    colliderEntity, *otherCollider) &&
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
    std::shared_ptr<Registry> registry,
    size_t entityA,
    const ColliderComponent& colliderA,
    size_t entityB,
    const ColliderComponent& colliderB
) {
    for (const auto& excludeCondition : colliderA.getExcludeTags()) {
        if (entityMatchesCondition(registry, entityB, excludeCondition)) {
            return false;
        }
    }

    for (const auto& excludeCondition : colliderB.getExcludeTags()) {
        if (entityMatchesCondition(registry, entityA, excludeCondition)) {
            return false;
        }
    }

    if (!colliderA.getIncludeTags().empty()) {
        bool matchesAny = false;
        for (const auto& includeCondition : colliderA.getIncludeTags()) {
            if (entityMatchesCondition(registry, entityB, includeCondition)) {
                matchesAny = true;
                break;
            }
        }
        if (!matchesAny) {
            return false;
        }
    }

    if (!colliderB.getIncludeTags().empty()) {
        bool matchesAny = false;
        for (const auto& includeCondition : colliderB.getIncludeTags()) {
            if (entityMatchesCondition(registry, entityA, includeCondition)) {
                matchesAny = true;
                break;
            }
        }
        if (!matchesAny) {
            return false;
        }
    }

    return true;
}

bool TriggerSystem::entityMatchesCondition(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    const std::vector<std::string>& condition
) {
    const TagRegistry& tagRegistry = TagRegistry::getInstance();
    for (const auto& tag : condition) {
        if (!tagRegistry.hasTag(registry, entityId, tag)) {
            return false;
        }
    }
    return true;
}

}  // namespace ecs
