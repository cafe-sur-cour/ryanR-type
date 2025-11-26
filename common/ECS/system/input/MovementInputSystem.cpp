/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#include "MovementInputSystem.hpp"
#include <cmath>
#include <memory>
#include "../../../types/Vector2f.hpp"
#include "../../../constants.hpp"
#include "../../component/tags/PlayerTag.hpp"

namespace ecs {

MovementInputSystem::MovementInputSystem() {
    // TEMPORARY: Will be useless when real input system is implemented
    _movementKeyStates[MovementKey::Left] = false;
    _movementKeyStates[MovementKey::Right] = false;
    _movementKeyStates[MovementKey::Up] = false;
    _movementKeyStates[MovementKey::Down] = false;
    _axisInput = math::Vector2f(0.0f, 0.0f);
}

void MovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<ARegistry> registry,
    float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<PlayerTag>();
    math::Vector2f movementDirection = getMovementDirection();

    if (std::fabs(movementDirection.getX()) > constants::EPS ||
        std::fabs(movementDirection.getY()) > constants::EPS) {
        for (auto entityId : view) {
            updateMovementIntent(registry, entityId, movementDirection);
        }
    }
}

void MovementInputSystem::simulateKeyPress(MovementKey key, bool pressed) {
    _movementKeyStates[key] = pressed;
}

void MovementInputSystem::simulateAxis(float horizontal, float vertical) {
    _axisInput = math::Vector2f(horizontal, vertical);
}

math::Vector2f MovementInputSystem::getMovementDirection() const {
    math::Vector2f direction(0.0f, 0.0f);

    // TEMPORARY: Simulate input based on key states
    // will be replaced by real input system
    if (_movementKeyStates.at(MovementKey::Left))
        direction.setX(direction.getX() - 1.0f);
    if (_movementKeyStates.at(MovementKey::Right))
        direction.setX(direction.getX() + 1.0f);
    if (_movementKeyStates.at(MovementKey::Up))
        direction.setY(direction.getY() - 1.0f);
    if (_movementKeyStates.at(MovementKey::Down))
        direction.setY(direction.getY() + 1.0f);

    // will be replaced by real input system
    if (std::fabs(_axisInput.getX()) > constants::EPS ||
        std::fabs(_axisInput.getY()) > constants::EPS) {
        direction = _axisInput;
    }

    /* Normalize direction */
    if (std::fabs(direction.getX()) > constants::EPS &&
        std::fabs(direction.getY()) > constants::EPS) {
        float length = std::sqrt(direction.getX() * direction.getX() +
                                  direction.getY() * direction.getY());
        if (length > 1.0f) {
            direction.setX(direction.getX() / length);
            direction.setY(direction.getY() / length);
        }
    }

    return direction;
}

void MovementInputSystem::updateMovementIntent(
    std::shared_ptr<ARegistry> registry,
    size_t entityId,
    const math::Vector2f &direction) {

    registry->registerComponent<MovementIntentComponent>();
    auto movementIntent = std::make_shared<MovementIntentComponent>(direction,
                                                                     true);

    if (registry->hasComponent<MovementIntentComponent>(entityId)) {
        auto existingIntent = registry->getComponent<MovementIntentComponent>(
            entityId);
        existingIntent->setDirection(direction);
        existingIntent->setActive(true);
        existingIntent->setState(ComponentState::Temporary);
    } else {
        registry->addComponent(entityId, movementIntent);
    }
}

}  // namespace ecs
