/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementSystem
*/

#include "MovementSystem.hpp"

namespace ecs {

MovementSystem::MovementSystem() {
}

void MovementSystem::update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) {
    (void)resourceManager;

    auto view = registry->view<MovementIntentComponent, TransformComponent>();

    for (auto entityId : view) {
        auto intent = registry->getComponent<MovementIntentComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!intent->isActive()) {
            continue;
        }

        math::Vector2f direction = intent->getDirection();

        float speed = constants::BASE_SPEED;
        if (registry->hasComponent<SpeedComponent>(entityId)) {
            auto speedComp = registry->getComponent<SpeedComponent>(entityId);
            speed = speedComp->getSpeed();
        }

        math::Vector2f currentPos = transform->getPosition();
        math::Vector2f newPos = currentPos + direction * speed * deltaTime;
        transform->setPosition(newPos);

        intent->setState(ComponentState::Processed);
    }
}

}  // namespace ecs
