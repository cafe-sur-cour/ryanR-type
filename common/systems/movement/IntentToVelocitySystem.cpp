/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IntentToVelocitySystem
*/

#include <memory>
#include "IntentToVelocitySystem.hpp"
#include "../../components/temporary/MovementIntentComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"

namespace ecs {

IntentToVelocitySystem::IntentToVelocitySystem() {
}

void IntentToVelocitySystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {

    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<MovementIntentComponent>();

    for (auto entityId : view) {
        auto movementIntent =
            registry->getComponent<MovementIntentComponent>(entityId);

        if (!movementIntent->isActive()) {
            continue;
        }

        math::Vector2f direction = movementIntent->getDirection();

        registry->registerComponent<VelocityComponent>();
        math::Vector2f velocity = direction;

        if (registry->hasComponent<VelocityComponent>(entityId)) {
            auto existingVelocity =
                registry->getComponent<VelocityComponent>(entityId);
            existingVelocity->setVelocity(velocity);
        } else {
            auto velocityComp = std::make_shared<VelocityComponent>(velocity);
            registry->addComponent(entityId, velocityComp);
        }
        registry->removeOneComponent<MovementIntentComponent>(entityId);
    }
}

}  // namespace ecs
