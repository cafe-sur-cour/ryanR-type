/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/

#include "MovementInputSystem.hpp"
#include <cmath>
#include <memory>
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/ImputMapping/IInputProvider.hpp"
#include "../../../common/ImputMapping/InputAction.hpp"

namespace ecs {

MovementInputSystem::MovementInputSystem() {
}

void MovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<ControllableTag>();
    math::Vector2f movementDirection = getMovementDirection(resourceManager);

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

    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    const float maxValue = 100.0f;
    const float range = maxValue - deadzone;

    if (std::abs(rawX) < deadzone) {
        rawX = 0.0f;
    } else {
        float sign = rawX > 0 ? 1.0f : -1.0f;
        rawX = (std::abs(rawX) - deadzone) / range * sign;
    }

    if (std::abs(rawY) < deadzone) {
        rawY = 0.0f;
    } else {
        float sign = rawY > 0 ? 1.0f : -1.0f;
        rawY = (std::abs(rawY) - deadzone) / range * sign;
    }

    math::Vector2f normalized(rawX, rawY);

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
