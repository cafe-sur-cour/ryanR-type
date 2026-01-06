/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementInputSystem
*/
#include <cmath>

#include <iostream>
#include <memory>
#include <utility>

#include "MovementInputSystem.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../common/components/permanent/HealthComponent.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/systems/input/InputNormalizer.hpp"
#include "../../ClientNetwork.hpp"

namespace ecs {

MovementInputSystem::MovementInputSystem() {
}

void MovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<ControllableTag, LocalPlayerTag>();

    bool localPlayerAlive = false;
    for (auto entityId : view) {
        if (isPlayerAlive(registry, entityId)) {
            localPlayerAlive = true;
            break;
        }
    }
    if (localPlayerAlive) {
        math::Vector2f movementDirection = getMovementDirection(resourceManager);

        bool isMovingThisFrame = !(std::fabs(movementDirection.getX()) <= constants::EPS &&
            std::fabs(movementDirection.getY()) <= constants::EPS);

        if (resourceManager->has<ClientNetwork>()) {
            if (isMovingThisFrame) {
                sendAxisEvents(resourceManager, movementDirection);
                _wasMovingLastFrame = true;
            } else if (_wasMovingLastFrame) {
                NetworkEvent stopEvent;
                stopEvent.eventType = constants::EventType::STOP;
                stopEvent.depth = 0.0;
                resourceManager->get<ClientNetwork>()->addToEventQueue(stopEvent);
                _wasMovingLastFrame = false;
            }
        }
        for (auto entityId : view) {
            if (isPlayerAlive(registry, entityId)) {
                updateInputIntent(registry, entityId, movementDirection);
            }
        }
    } else {
        _wasMovingLastFrame = false;
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
    direction = InputNormalizer::normalizeDirection(direction);

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

    return InputNormalizer::normalizeAnalogInput(rawX, rawY);
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

void MovementInputSystem::sendAxisEvents(
    std::shared_ptr<ResourceManager> resourceManager,
    const math::Vector2f &direction) {
    auto clientNetwork = resourceManager->get<ClientNetwork>();
    if (std::fabs(direction.getX()) > constants::EPS) {
        constants::EventType xEventType;
        if (direction.getX() > 0) {
            xEventType = constants::EventType::RIGHT;
        } else {
            xEventType = constants::EventType::LEFT;
        }
        NetworkEvent xEvent;
        xEvent.eventType = xEventType;
        xEvent.depth = static_cast<double>(std::fabs(direction.getX()));
        clientNetwork->addToEventQueue(xEvent);
    }
    if (std::fabs(direction.getY()) > constants::EPS) {
        constants::EventType yEventType;
        if (direction.getY() < 0) {
            yEventType = constants::EventType::UP;
        } else {
            yEventType = constants::EventType::DOWN;
        }
        NetworkEvent yEvent;
        yEvent.eventType = yEventType;
        yEvent.depth = static_cast<double>(std::fabs(direction.getY()));
        clientNetwork->addToEventQueue(yEvent);
    }
}

bool MovementInputSystem::isPlayerAlive(
    std::shared_ptr<Registry> registry,
    Entity entityId) const {
    if (registry->hasComponent<HealthComponent>(entityId)) {
        auto health = registry->getComponent<HealthComponent>(entityId);
        return health->getHealth() > 0.0f;
    }
    return true;
}

}  // namespace ecs
