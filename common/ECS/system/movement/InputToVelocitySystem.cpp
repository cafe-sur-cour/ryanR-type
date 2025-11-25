/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** VelocitySystem
*/

#include "InputToVelocitySystem.hpp"
#include "../../component/temporary/InputIntentComponent.hpp"
#include "../../component/permanent/VelocityComponent.hpp"
#include "../../../constants.hpp"

namespace ecs {

InputToVelocitySystem::InputToVelocitySystem() {
}

void InputToVelocitySystem::update(std::shared_ptr<ResourceManager> resourceManager,
                             std::shared_ptr<ARegistry> registry,
                             float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<InputIntentComponent>();

    for (auto entityId : view) {
        auto inputIntent = registry->getComponent<InputIntentComponent>(entityId);
        math::Vector2f direction = inputIntent->getDirection();

        registry->registerComponent<VelocityComponent>();
        math::Vector2f velocity = direction * constants::BASE_SPEED;

        if (registry->hasComponent<VelocityComponent>(entityId)) {
            auto existingVelocity = registry->getComponent<VelocityComponent>(entityId);
            existingVelocity->setVelocity(velocity);
        } else {
            auto velocityComp = std::make_shared<VelocityComponent>(velocity);
            registry->addComponent(entityId, velocityComp);
        }

        inputIntent->setState(ComponentState::Processed);
    }
}

}  // namespace ecs
