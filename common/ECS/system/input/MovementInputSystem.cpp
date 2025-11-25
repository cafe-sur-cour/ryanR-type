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
#include "../../../../client/graphicals/IEvent.hpp"
#include "../../../constants.hpp"

namespace ecs {

MovementInputSystem::MovementInputSystem() {
}

void MovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<ARegistry> registry,
    float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<PlayerTag>();
    math::Vector2f movementDirection = getMovementDirection(resourceManager);

    for (auto entityId : view) {
        updateInputIntent(registry, entityId, movementDirection);
    }
}

math::Vector2f MovementInputSystem::getMovementDirection(
    std::shared_ptr<ResourceManager> resourceManager) const {
    math::Vector2f direction(0.0f, 0.0f);

    if (!resourceManager->has<gfx::IEvent>()) {
        return direction;
    }

    auto eventSystem = resourceManager->get<gfx::IEvent>();

    /* Keyboard input */
    if (eventSystem->isKeyPressed(gfx::EventType::LEFT))
        direction.setX(direction.getX() - 1.0f);
    if (eventSystem->isKeyPressed(gfx::EventType::RIGHT))
        direction.setX(direction.getX() + 1.0f);
    if (eventSystem->isKeyPressed(gfx::EventType::UP))
        direction.setY(direction.getY() - 1.0f);
    if (eventSystem->isKeyPressed(gfx::EventType::DOWN))
        direction.setY(direction.getY() + 1.0f);

    /* Normalize keyboard input */
    float magnitude = std::sqrt(direction.getX() *
                    direction.getX() + direction.getY() * direction.getY());
    if (magnitude > 1.0f) {
        direction.setX(direction.getX() / magnitude);
        direction.setY(direction.getY() / magnitude);
    }

    /* Gamepad input */
    math::Vector2f analogInput = getAnalogStickInput(eventSystem);
    if (analogInput.getX() != 0.0f || analogInput.getY() != 0.0f) {
        direction = analogInput;
    }

    return direction;
}

math::Vector2f MovementInputSystem::getAnalogStickInput(
    std::shared_ptr<gfx::IEvent> eventSystem) const {
    float rawX =
        eventSystem->getAxisValue(gfx::EventType::GAMEPAD_LEFT_STICK_RIGHT);
    float rawY =
        eventSystem->getAxisValue(gfx::EventType::GAMEPAD_LEFT_STICK_DOWN);

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
