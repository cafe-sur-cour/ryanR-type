/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerSystem
*/

#include "TriggerSystem.hpp"
#include <memory>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"

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
            if (otherCollider && checkCollision(*triggerTransform, *triggerCollider,
                    *colliderTransform, *otherCollider)) {
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

}  // namespace ecs
