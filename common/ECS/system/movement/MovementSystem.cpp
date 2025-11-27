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
#include "../../component/permanent/TransformComponent.hpp"
#include "../../component/permanent/ColliderComponent.hpp"
#include "../../../types/FRect.hpp"

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
        math::Vector2f desiredPos = currentPos + velocity * deltaTime;

        math::Vector2f finalPos = calculateSmoothSlidingPosition(
            registry, entityId, currentPos, desiredPos);
        transform->setPosition(finalPos);
    }
}

bool MovementSystem::checkCollision(
    std::shared_ptr<ARegistry> registry,
    size_t entityId,
    math::Vector2f newPos) {

    auto movingColliders = registry->getComponents<ColliderComponent>(entityId);
    if (movingColliders.empty()) {
        return true;
    }

    auto allEntitiesView = registry->view<
        TransformComponent, ColliderComponent>();
    for (auto otherEntityId : allEntitiesView) {
        if (otherEntityId == entityId) continue;

        auto otherTransform = registry->getComponent<
            TransformComponent>(otherEntityId);
        auto otherColliders = registry->getComponents<
            ColliderComponent>(otherEntityId);

        for (auto& movingCollider : movingColliders) {
            if (movingCollider->getType() != CollisionType::Solid) continue;

            math::FRect movingHitbox = movingCollider->getHitbox(newPos);

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid) continue;

                math::FRect otherHitbox =
                    otherCollider->getHitbox(otherTransform->getPosition());

                if (movingHitbox.intersects(otherHitbox)) {
                    return false;
                }
            }
        }
    }
    return true;
}

math::Vector2f MovementSystem::calculateSmoothMovement(
    std::shared_ptr<ARegistry> registry,
    size_t entityId,
    math::Vector2f startPos,
    math::Vector2f desiredPos) {

    if (checkCollision(registry, entityId, desiredPos)) {
        return desiredPos;
    }

    math::Vector2f movement = desiredPos - startPos;
    float distance = std::sqrt(movement.getX() *
        movement.getX() + movement.getY() * movement.getY());

    if (distance < 0.001f) {
        return startPos;
    }

    math::Vector2f direction(movement.getX() /
        distance, movement.getY() / distance);

    float minDist = 0.0f;
    float maxDist = distance;
    float bestDist = 0.0f;

    for (int i = 0; i < 4; ++i) {
        float testDist = (minDist + maxDist) / 2.0f;
        math::Vector2f testPos = startPos + direction * testDist;

        if (checkCollision(registry, entityId, testPos)) {
            bestDist = testDist;
            minDist = testDist;
        } else {
            maxDist = testDist;
        }
    }

    return startPos + direction * bestDist;
}

math::Vector2f MovementSystem::calculateSlidingMovement(
    std::shared_ptr<ARegistry> registry,
    size_t entityId,
    math::Vector2f basePos,
    math::Vector2f desiredPos) {

    math::Vector2f remainingMovement = desiredPos - basePos;
    math::Vector2f slidePos = basePos;

    if (std::abs(remainingMovement.getX()) > 0.001f) {
        math::Vector2f horizontalSlide =
            basePos + math::Vector2f(remainingMovement.getX(), 0.0f);
        if (checkCollision(registry, entityId, horizontalSlide)) {
            slidePos.setX(horizontalSlide.getX());
        }
    }

    if (std::abs(remainingMovement.getY()) > 0.001f) {
        math::Vector2f verticalSlide =
            basePos + math::Vector2f(0.0f, remainingMovement.getY());
        if (checkCollision(registry, entityId, verticalSlide)) {
            slidePos.setY(verticalSlide.getY());
        }
    }

    return slidePos;
}

math::Vector2f MovementSystem::calculateSmoothSlidingPosition(
    std::shared_ptr<ARegistry> registry,
    size_t entityId,
    math::Vector2f startPos,
    math::Vector2f desiredPos) {

    math::Vector2f smoothPos =
        calculateSmoothMovement(registry, entityId, startPos, desiredPos);

    return calculateSlidingMovement(registry, entityId, smoothPos, desiredPos);
}

}  // namespace ecs
