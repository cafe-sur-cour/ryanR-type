/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementSystem
*/

#include <memory>
#include <cmath>
#include "MovementSystem.hpp"
#include "../../component/permanent/VelocityComponent.hpp"

namespace ecs {

MovementSystem::MovementSystem() {
}

void MovementSystem::update(std::shared_ptr<ResourceManager> resourceManager,
                             std::shared_ptr<ARegistry> registry,
                             float deltaTime) {
    (void)resourceManager;

    auto view = registry->view<VelocityComponent, TransformComponent>();

    for (auto entityId : view) {
        auto velocityComp = registry->getComponent<VelocityComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        math::Vector2f velocity = velocityComp->getVelocity();
        math::Vector2f currentPos = transform->getPosition();
        math::Vector2f newPos = currentPos + velocity * deltaTime;
        transform->setPosition(newPos);
    }
}

}  // namespace ecs
