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
#include "../../component/tags/PlayerTag.hpp"
#include "../../component/temporary/InputIntentComponent.hpp"

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

    for (auto entityId : view) {
        updateInputIntent(registry, entityId, movementDirection);
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
    if (_axisInput.getX() != 0.0f || _axisInput.getY() != 0.0f) {
        direction = _axisInput;
    }

    /* Normalize direction */
    if (direction.getX() != 0.0f && direction.getY() != 0.0f) {
        float length = std::sqrt(direction.getX() * direction.getX() +
                                  direction.getY() * direction.getY());
        if (length > 1.0f) {
            direction.setX(direction.getX() / length);
            direction.setY(direction.getY() / length);
        }
    }

    return direction;
}

void MovementInputSystem::updateInputIntent(
    std::shared_ptr<ARegistry> registry,
    int entityId,
    const math::Vector2f &direction) {

    registry->registerComponent<InputIntentComponent>();
    auto inputIntent = std::make_shared<InputIntentComponent>(direction);

    if (registry->hasComponent<InputIntentComponent>(entityId)) {
        auto existingIntent =
            registry->getComponent<InputIntentComponent>(entityId);
        existingIntent->setDirection(direction);
        existingIntent->setState(ComponentState::Temporary);
    } else {
        registry->addComponent(entityId, inputIntent);
    }
}

}  // namespace ecs
