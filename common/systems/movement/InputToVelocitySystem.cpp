/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** VelocitySystem
*/

#include <memory>
#include "InputToVelocitySystem.hpp"
#include "../../components/temporary/InputIntentComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../constants.hpp"

namespace ecs {

InputToVelocitySystem::InputToVelocitySystem() {
}

void InputToVelocitySystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {

    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<InputIntentComponent, SpeedComponent>();

    for (auto entityId : view) {
        auto inputIntent =
            registry->getComponent<InputIntentComponent>(entityId);
        auto speedComp = registry->getComponent<SpeedComponent>(entityId);
        math::Vector2f direction = inputIntent->getDirection();

        registry->registerComponent<VelocityComponent>();
        math::Vector2f velocity = direction * speedComp->getSpeed();

        if (registry->hasComponent<VelocityComponent>(entityId)) {
            auto existingVelocity =
                registry->getComponent<VelocityComponent>(entityId);
            existingVelocity->setVelocity(velocity);
        } else {
            auto velocityComp = std::make_shared<VelocityComponent>(velocity);
            registry->addComponent(entityId, velocityComp);
        }

        registry->removeOneComponent<InputIntentComponent>(entityId);
    }
}

}  // namespace ecs
