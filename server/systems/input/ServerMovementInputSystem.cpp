/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerMovementInputSystem
*/

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include "ServerMovementInputSystem.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/systems/input/InputNormalizer.hpp"

namespace ecs {

ServerMovementInputSystem::ServerMovementInputSystem() {
}

math::Vector2f ServerMovementInputSystem::normalizeDirection
(const math::Vector2f &direction) const {
    return InputNormalizer::normalizeDirection(direction);
}

void ServerMovementInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)deltaTime;

    if (!resourceManager->has<ecs::ServerInputProvider>()) {
        return;
    }

    auto serverInputProvider = resourceManager->get<ecs::ServerInputProvider>();

    auto view = registry->view<ControllableTag, PlayerTag>();

    for (auto entityId : view) {
        size_t clientID = serverInputProvider->getClientIdForEntity(entityId);
        if (clientID == 0) {
            continue;
        }
        float axisX = serverInputProvider->getActionAxis(InputAction::MOVE_X, clientID);
        float axisY = serverInputProvider->getActionAxis(InputAction::MOVE_Y, clientID);
        math::Vector2f movementDirection(axisX, axisY);
        updateInputIntent(registry, entityId, movementDirection);
    }
}

math::Vector2f ServerMovementInputSystem::getMovementDirection(
    std::shared_ptr<IInputProvider> inputProvider, size_t clientID
) const {
    math::Vector2f direction(0.0f, 0.0f);
    direction.setX(inputProvider->getActionAxis(InputAction::MOVE_X, clientID));
    direction.setY(inputProvider->getActionAxis(InputAction::MOVE_Y, clientID));

    direction = normalizeDirection(direction);
    math::Vector2f analogInput = getAnalogStickInput(inputProvider, clientID);
    if (std::fabs(analogInput.getX()) > constants::EPS ||
        std::fabs(analogInput.getY()) > constants::EPS) {
        direction = analogInput;
    }

    return direction;
}

math::Vector2f ServerMovementInputSystem::getAnalogStickInput(
    std::shared_ptr<IInputProvider> inputProvider, size_t clientID) const {
    (void)inputProvider;
    (void)clientID;
    return math::Vector2f(0.0f, 0.0f);
}

void ServerMovementInputSystem::updateInputIntent(
    std::shared_ptr<Registry> registry,
    Entity entityId,
    const math::Vector2f &direction) {

    registry->registerComponent<InputIntentComponent>();
    auto inputIntent = std::make_shared<InputIntentComponent>(direction);

    if (registry->hasComponent<InputIntentComponent>(entityId)) {
        auto existingIntent = registry->getComponent<InputIntentComponent>(entityId);

        existingIntent->setDirection(direction);
    } else {
        registry->addComponent(entityId, inputIntent);
    }
}

}  // namespace ecs
