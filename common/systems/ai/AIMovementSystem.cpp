/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIMovementSystem
*/

#include "AIMovementSystem.hpp"
#include <cmath>
#include <memory>
#include "../../constants.hpp"
#include "../../components/temporary/InputIntentComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/tags/AIMoverTag.hpp"
#include "../SystemNames.hpp"

namespace ecs {

void AIMovementSystem::update(std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void) resourceManager;
    auto view = registry->view<AIMoverTag, AIMovementPatternComponent, TransformComponent>();
    for (auto entityId : view) {
        auto movement = registry->getComponent<AIMovementPatternComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);
        if (!movement || !transform) continue;
        if (!registry->hasComponent<InputIntentComponent>(entityId)) {
            registry->addComponent<InputIntentComponent>(entityId,
                std::make_shared<InputIntentComponent>());
        }
        auto inputIntent = registry->getComponent<InputIntentComponent>(entityId);
        switch (movement->pattern) {
            case AIMovementPattern::STRAIGHT_LINE:
                executeStraightLine(movement, transform, inputIntent);
                break;
            case AIMovementPattern::ZIGZAG:
                executeZigzag(movement, transform, inputIntent, deltaTime);
                break;
            case AIMovementPattern::VERTICAL_MIRROR:
                executeVerticalMirror(registry, movement, transform, inputIntent);
                break;
            case AIMovementPattern::FOLLOW_RIGHT:
                executeFollowRight(movement, transform, inputIntent, deltaTime);
                break;
        }
    }
}

void AIMovementSystem::executeStraightLine(
    std::shared_ptr<AIMovementPatternComponent> movement,
    std::shared_ptr<TransformComponent> transform,
    std::shared_ptr<InputIntentComponent> inputIntent) {
    (void) movement;
    (void) transform;
    math::Vector2f direction(-1.0f, 0.0f);
    inputIntent->setDirection(direction);
}

void AIMovementSystem::executeZigzag(
    std::shared_ptr<AIMovementPatternComponent> movement,
    std::shared_ptr<TransformComponent> transform,
    std::shared_ptr<InputIntentComponent> inputIntent,
    float deltaTime) {
    (void) transform;
    movement->timer += deltaTime * movement->zigzagFrequency;
    float verticalDirection = std::cos(movement->timer) * movement->zigzagFrequency;
    math::Vector2f direction(-1.0f, verticalDirection);
    float length = std::sqrt(direction.getX() * direction.getX() +
                            direction.getY() * direction.getY());
    if (length > constants::EPS) {
        direction = math::Vector2f(direction.getX() / length, direction.getY() / length);
    }
    inputIntent->setDirection(direction);
}

void AIMovementSystem::executeVerticalMirror(
    std::shared_ptr<Registry> registry,
    std::shared_ptr<AIMovementPatternComponent> movement,
    std::shared_ptr<TransformComponent> transform,
    std::shared_ptr<InputIntentComponent> inputIntent) {
    auto playerTarget = findNearestPlayer(registry, transform->getPosition(),
        movement->detectionRange);
    if (!playerTarget.has_value()) {
        math::Vector2f direction(-1.0f, 0.0f);
        inputIntent->setDirection(direction);
        return;
    }
    auto targetTransform = registry->getComponent<TransformComponent>(*playerTarget);
    if (!targetTransform) {
        math::Vector2f direction(-1.0f, 0.0f);
        inputIntent->setDirection(direction);
        return;
    }
    float enemyY = transform->getPosition().getY();
    float playerY = targetTransform->getPosition().getY();
    float deltaY = playerY - enemyY;
    float verticalDir = 0.0f;
    if (std::abs(deltaY) > movement->verticalDeadzone) {
        verticalDir = (deltaY > 0) ? 1.0f : -1.0f;
    }
    math::Vector2f direction(-1.0f, verticalDir);
    float length = std::sqrt(direction.getX() * direction.getX() +
                            direction.getY() * direction.getY());
    if (length > constants::EPS) {
        direction = math::Vector2f(direction.getX() / length, direction.getY() / length);
    }
    inputIntent->setDirection(direction);
}

void AIMovementSystem::executeFollowRight(
    std::shared_ptr<AIMovementPatternComponent> movement,
    std::shared_ptr<TransformComponent> transform,
    std::shared_ptr<InputIntentComponent> inputIntent,
    float deltaTime
) {
    (void) transform;
    movement->timer += deltaTime * movement->zigzagFrequency;
    float verticalDirection = std::cos(movement->timer) * movement->zigzagFrequency;
    math::Vector2f direction(1.0f, verticalDirection);
    float length = std::sqrt(
        direction.getX() * direction.getX() +
        direction.getY() * direction.getY()
    );
    if (length > constants::EPS) {
        direction = math::Vector2f(direction.getX() / length, direction.getY() / length);
    }
    inputIntent->setDirection(direction);
}

std::optional<size_t> AIMovementSystem::findNearestPlayer(
    std::shared_ptr<Registry> registry,
    const math::Vector2f& position,
    float range) {
    auto playerView = registry->view<PlayerTag, TransformComponent>();
    std::optional<size_t> nearestPlayer;
    float nearestDistance = range * range;
    for (auto playerId : playerView) {
        auto playerTransform = registry->getComponent<TransformComponent>(playerId);
        if (!playerTransform)
            continue;
        auto playerPos = playerTransform->getPosition();
        float dx = playerPos.getX() - position.getX();
        float dy = playerPos.getY() - position.getY();
        float distSquared = dx * dx + dy * dy;
        if (distSquared < nearestDistance) {
            nearestDistance = distSquared;
            nearestPlayer = playerId;
        }
    }
    return nearestPlayer;
}

}  // namespace ecs


extern "C" ecs::ISystem* createSystem() {
    return new ecs::AIMovementSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::AI_MOVEMENT_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
