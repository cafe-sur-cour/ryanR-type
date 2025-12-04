/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementSystem
*/

#include <memory>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "MovementSystem.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../types/FRect.hpp"
#include "../../constants.hpp"
#include "../../components/tags/ObstacleTag.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../components/tags/ProjectilePassThroughTag.hpp"
#include "../../components/tags/GameZoneColliderTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"

namespace ecs {

MovementSystem::MovementSystem() {
}

void MovementSystem::update(std::shared_ptr<ResourceManager> resourceManager,
                             std::shared_ptr<Registry> registry,
                             float deltaTime) {
    (void)resourceManager;

    auto view = registry->view<VelocityComponent, TransformComponent>();

    for (auto entityId : view) {
        auto velocityComp = registry->getComponent<VelocityComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        math::Vector2f velocity = velocityComp->getVelocity();
        math::Vector2f currentPos = transform->getPosition();
        math::Vector2f desiredPos = currentPos + velocity * deltaTime;

        auto colliders = registry->getComponents<ColliderComponent>(entityId);
        bool hasBounceCollider = false;
        bool hasPushCollider = false;
        for (auto& collider : colliders) {
            if (collider->getType() == CollisionType::Bounce) {
                hasBounceCollider = true;
            }
            if (collider->getType() == CollisionType::Push) {
                hasPushCollider = true;
            }
        }

        if (hasBounceCollider) {
            math::Vector2f finalPos = handleBounceCollision(
                registry, entityId, currentPos, desiredPos, velocityComp);
            transform->setPosition(finalPos);
            if (hasPushCollider) {
                handlePushCollision(registry, entityId, finalPos, deltaTime);
            }
        } else {
            math::Vector2f finalPos = calculateSmoothSlidingPosition(
                registry, entityId, currentPos, desiredPos);
            transform->setPosition(finalPos);

            if (registry->hasComponent<ProjectileTag>(entityId) &&
                (std::abs(finalPos.getX() - desiredPos.getX()) > constants::EPS ||
                 std::abs(finalPos.getY() - desiredPos.getY()) > constants::EPS)) {
                velocityComp->setVelocity(math::Vector2f(0.0f, 0.0f));
            }

            if (hasPushCollider) {
                handlePushCollision(registry, entityId, finalPos, deltaTime);
            }
        }
    }
}

bool MovementSystem::checkCollision(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f newPos) {

    auto movingColliders = registry->getComponents<ColliderComponent>(entityId);
    if (movingColliders.empty()) {
        return true;
    }

    bool isProjectile = registry->hasComponent<ProjectileTag>(entityId);

    auto movingTransform = registry->getComponent<TransformComponent>(entityId);
    math::Vector2f movingScale = movingTransform->getScale();

    auto allEntitiesView = registry->view<
        TransformComponent, ColliderComponent>();
    for (auto otherEntityId : allEntitiesView) {
        if (otherEntityId == entityId) continue;

        auto otherTransform = registry->getComponent<
            TransformComponent>(otherEntityId);
        auto otherColliders = registry->getComponents<
            ColliderComponent>(otherEntityId);

        bool otherIsProjectile = registry->hasComponent<ProjectileTag>(otherEntityId);

        if ((isProjectile && registry->hasComponent<
                ProjectilePassThroughTag>(otherEntityId)) ||
            (otherIsProjectile && registry->hasComponent<
                ProjectilePassThroughTag>(entityId)))
            continue;

        for (auto& movingCollider : movingColliders) {
            if (movingCollider->getType() != CollisionType::Solid) continue;

            math::FRect movingHitbox = movingCollider->getHitbox(newPos, movingScale);

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid &&
                    otherCollider->getType() != CollisionType::Push) continue;

                math::Vector2f otherScale = otherTransform->getScale();
                math::FRect otherHitbox =
                    otherCollider->getHitbox(otherTransform->getPosition(), otherScale);

                bool isGameZoneCollision = registry->hasComponent<
                    GameZoneColliderTag>(otherEntityId) ||
                    registry->hasComponent<GameZoneColliderTag>(entityId);
                bool isPlayerInvolved = registry->hasComponent<PlayerTag>(entityId) ||
                                       registry->hasComponent<PlayerTag>(otherEntityId);

                if (isGameZoneCollision && !isPlayerInvolved) {
                    continue;
                }

                if (movingHitbox.intersects(otherHitbox)) {
                    return false;
                }
            }
        }
    }
    return true;
}

math::Vector2f MovementSystem::calculateSmoothMovement(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f startPos,
    math::Vector2f desiredPos) {

    if (checkCollision(registry, entityId, desiredPos)) {
        return desiredPos;
    }

    math::Vector2f movement = desiredPos - startPos;
    float distance = std::sqrt(movement.getX() *
        movement.getX() + movement.getY() * movement.getY());

    if (distance < constants::EPS) {
        return startPos;
    }

    math::Vector2f direction(movement.getX() /
        distance, movement.getY() / distance);

    float minDist = 0.0f;
    float maxDist = distance;
    float bestDist = 0.0f;

    for (int i = 0; i < constants::SMOOTH_MOVEMENT_ITERATIONS; ++i) {
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
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f basePos,
    math::Vector2f desiredPos) {

    math::Vector2f remainingMovement = desiredPos - basePos;
    math::Vector2f slidePos = basePos;

    if (std::abs(remainingMovement.getX()) > constants::EPS) {
        math::Vector2f horizontalSlide =
            basePos + math::Vector2f(remainingMovement.getX(), 0.0f);
        if (checkCollision(registry, entityId, horizontalSlide)) {
            slidePos.setX(horizontalSlide.getX());
        }
    }

    if (std::abs(remainingMovement.getY()) > constants::EPS) {
        math::Vector2f verticalSlide =
            basePos + math::Vector2f(0.0f, remainingMovement.getY());
        if (checkCollision(registry, entityId, verticalSlide)) {
            slidePos.setY(verticalSlide.getY());
        }
    }

    return slidePos;
}

math::Vector2f MovementSystem::calculateSmoothSlidingPosition(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f startPos,
    math::Vector2f desiredPos) {

    math::Vector2f smoothPos =
        calculateSmoothMovement(registry, entityId, startPos, desiredPos);

    return calculateSlidingMovement(registry, entityId, smoothPos, desiredPos);
}

math::Vector2f MovementSystem::handleBounceCollision(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f startPos,
    math::Vector2f desiredPos,
    std::shared_ptr<ecs::VelocityComponent> velocityComp
) {
    auto bounceColliders = registry->getComponents<ColliderComponent>(entityId);
    if (bounceColliders.empty()) {
        return desiredPos;
    }

    auto entityTransform = registry->getComponent<TransformComponent>(entityId);
    math::Vector2f entityScale = entityTransform->getScale();

    auto allEntitiesView = registry->view<TransformComponent, ColliderComponent>();

    for (auto otherEntityId : allEntitiesView) {
        if (otherEntityId == entityId) continue;

        auto otherTransform = registry->getComponent<TransformComponent>(otherEntityId);
        auto otherColliders = registry->getComponents<ColliderComponent>(otherEntityId);

        if (!registry->hasComponent<ObstacleTag>(otherEntityId)) continue;

        for (auto& bounceCollider : bounceColliders) {
            if (bounceCollider->getType() != CollisionType::Bounce) continue;

            math::FRect bounceHitbox = bounceCollider->getHitbox(desiredPos, entityScale);

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid) continue;

                math::Vector2f otherScale = otherTransform->getScale();
                math::FRect otherHitbox =
                    otherCollider->getHitbox(otherTransform->getPosition(), otherScale);

                if (bounceHitbox.intersects(otherHitbox)) {
                    math::Vector2f currentVelocity = velocityComp->getVelocity();
                    math::Vector2f newVelocity = currentVelocity;

                    float bounceRight = bounceHitbox.getLeft() + bounceHitbox.getWidth();
                    float bounceBottom = bounceHitbox.getTop() + bounceHitbox.getHeight();
                    float otherRight = otherHitbox.getLeft() + otherHitbox.getWidth();
                    float otherBottom = otherHitbox.getTop() + otherHitbox.getHeight();

                    float overlapX = std::min(bounceRight, otherRight) -
                        std::max(bounceHitbox.getLeft(), otherHitbox.getLeft());
                    float overlapY = std::min(bounceBottom, otherBottom) -
                        std::max(bounceHitbox.getTop(), otherHitbox.getTop());

                    if (overlapX < overlapY)
                        newVelocity.setX(-newVelocity.getX());
                    else
                        newVelocity.setY(-newVelocity.getY());

                    velocityComp->setVelocity(newVelocity);
                    return startPos;
                }
            }
        }
    }

    return calculateSmoothSlidingPosition(
        registry, entityId, startPos, desiredPos);
}

void MovementSystem::handlePushCollision(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f finalPos,
    float deltaTime) {

    auto pushColliders = registry->getComponents<ColliderComponent>(entityId);
    if (pushColliders.empty()) {
        return;
    }

    auto entityTransform = registry->getComponent<TransformComponent>(entityId);
    math::Vector2f entityScale = entityTransform->getScale();

    auto allEntitiesView = registry->view<TransformComponent, ColliderComponent>();

    for (auto otherEntityId : allEntitiesView) {
        if (otherEntityId == entityId) continue;

        auto otherTransform = registry->getComponent<TransformComponent>(otherEntityId);
        auto otherColliders = registry->getComponents<ColliderComponent>(otherEntityId);

        if (registry->hasComponent<GameZoneColliderTag>(entityId)) {
            if (!registry->hasComponent<PlayerTag>(otherEntityId)) {
                continue;
            }
        }

        for (auto& pushCollider : pushColliders) {
            if (pushCollider->getType() != CollisionType::Push) continue;

            math::FRect pushHitbox = pushCollider->getHitbox(finalPos, entityScale);

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() == CollisionType::None) continue;

                math::Vector2f otherScale = otherTransform->getScale();
                math::FRect otherHitbox =
                    otherCollider->getHitbox(otherTransform->getPosition(), otherScale);

                if (pushHitbox.intersects(otherHitbox)) {
                    auto pusherVelocity = registry->getComponent<VelocityComponent>(entityId);
                    math::Vector2f pushVelocity = pusherVelocity->getVelocity();
                    math::Vector2f pushAmount = pushVelocity * deltaTime;
                    math::Vector2f newOtherPos = otherTransform->getPosition() + pushAmount;

                    if (checkCollision(registry, otherEntityId, newOtherPos)) {
                        otherTransform->setPosition(newOtherPos);
                    } else {
                        if (registry->hasComponent<PlayerTag>(otherEntityId)) {
                            registry->addComponent<DeathIntentComponent>(otherEntityId,
                                std::make_shared<DeathIntentComponent>());
                        }
                    }
                }
            }
        }
    }
}

}  // namespace ecs
