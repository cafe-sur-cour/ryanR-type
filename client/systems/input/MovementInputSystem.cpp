/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#include <iostream>

#include "MovementInputSystem.hpp"
#include <cmath>
#include <memory>
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../../../common/ECS/resourceManager/InputAction.hpp"
#include "../../ClientNetwork.hpp"

namespace ecs {

MovementInputSystem::MovementInputSystem() {
}

std::pair<int, double> MovementInputSystem::formatDirection(const math::Vector2f &direction) const {

    float absX = std::abs(direction.getX());
    float absY = std::abs(direction.getY());

    int eventType;
    double depth;

    if (absY >= absX) {
        if (direction.getY() < 0) {
            eventType = static_cast<int>(constants::EventType::UP);
        } else {
            eventType = static_cast<int>(constants::EventType::DOWN);
        }
        depth = absY;
    }
    else {
        if (direction.getX() > 0) {
            eventType = static_cast<int>(constants::EventType::RIGHT);
        } else {
            eventType = static_cast<int>(constants::EventType::LEFT);
        }
        depth = absX;
    }
    depth = std::clamp(depth, 0.000, 1.0);
    return {eventType, depth};
}

void MovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<ControllableTag>();
    math::Vector2f movementDirection = getMovementDirection(resourceManager);

    if (resourceManager->has<ClientNetwork>() &&
        !(std::fabs(movementDirection.getX()) <= constants::EPS &&
        std::fabs(movementDirection.getY()) <= constants::EPS)) {
        std::pair<int, double> formatted =
            formatDirection(movementDirection);
        resourceManager->get<ClientNetwork>()->addToEventQueue({
            static_cast<constants::EventType>(formatted.first),
            formatted.second,
            0.0
        });
    }
    for (auto entityId : view) {
        updateInputIntent(registry, entityId, movementDirection);
    }
}

math::Vector2f MovementInputSystem::getMovementDirection(
    std::shared_ptr<ResourceManager> resourceManager
) const {
    math::Vector2f direction(0.0f, 0.0f);

    if (!resourceManager->has<IInputProvider>()) {
        return direction;
    }

    auto inputProvider = resourceManager->get<IInputProvider>();

    direction.setX(inputProvider->getActionAxis(InputAction::MOVE_X));
    direction.setY(inputProvider->getActionAxis(InputAction::MOVE_Y));

    /* Normalize keyboard/dpad input */
    float magnitude = std::sqrt(direction.getX() *
                    direction.getX() + direction.getY() * direction.getY());
    if (magnitude > 1.0f) {
        direction.setX(direction.getX() / magnitude);
        direction.setY(direction.getY() / magnitude);
    }

    /* Gamepad input */
    math::Vector2f analogInput = getAnalogStickInput(inputProvider);
    if (std::fabs(analogInput.getX()) > constants::EPS ||
        std::fabs(analogInput.getY()) > constants::EPS) {
        direction = analogInput;
    }

    return direction;
}

math::Vector2f MovementInputSystem::getAnalogStickInput(
    std::shared_ptr<IInputProvider> inputProvider) const {
    float rawX =
        inputProvider->getAxisValue(gfx::EventType::GAMEPAD_LEFT_STICK_RIGHT);
    float rawY =
        inputProvider->getAxisValue(gfx::EventType::GAMEPAD_LEFT_STICK_DOWN);

    /* Deadzone */
    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    if (std::abs(rawX) < deadzone) rawX = 0.0f;
    if (std::abs(rawY) < deadzone) rawY = 0.0f;

    math::Vector2f normalized(rawX / 100.0f, rawY / 100.0f);

    float magnitude = std::sqrt(normalized.getX() *
                    normalized.getX() + normalized.getY() * normalized.getY());
    if (magnitude > 1.0f) {
        normalized.setX(normalized.getX() / magnitude);
        normalized.setY(normalized.getY() / magnitude);
    }

    return normalized;
}

void MovementInputSystem::updateInputIntent(
    std::shared_ptr<Registry> registry,
    Entity entityId,
    const math::Vector2f &direction) {

    registry->registerComponent<InputIntentComponent>();
    auto inputIntent = std::make_shared<InputIntentComponent>(direction);

    if (registry->hasComponent<InputIntentComponent>(entityId)) {
        auto existingIntent =
            registry->getComponent<InputIntentComponent>(entityId);
        existingIntent->setDirection(direction);
    } else {
        registry->addComponent(entityId, inputIntent);
    }
}

}  // namespace ecs
