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
#include <string>
#include <vector>
#include "MovementSystem.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../types/FRect.hpp"
#include "../../types/OrientedRect.hpp"
#include "../../constants.hpp"
#include "../../components/tags/ObstacleTag.hpp"
#include "../../components/tags/GameZoneColliderTag.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../CollisionRules/CollisionRules.hpp"

namespace ecs {

MovementSystem::MovementSystem() : _spatialGrid(
    constants::MAX_WIDTH,
    constants::MAX_HEIGHT,
    constants::SPATIAL_GRID_CELL_SIZE,
    constants::SPATIAL_GRID_PADDING
) {
    this->_boundaryEntities = {};
}

void MovementSystem::buildSpatialGrid(std::shared_ptr<Registry> registry) {
    _spatialGrid.clear();
    _boundaryEntities.clear();

    auto gameZoneView = registry->view<GameZoneComponent, TransformComponent>();
    for (auto entityId : gameZoneView) {
        auto transform = registry->getComponent<TransformComponent>(entityId);
        if (transform) {
            math::Vector2f gameZonePos = transform->getPosition();
            _spatialGrid.setOffset(gameZonePos.getX(), gameZonePos.getY());
            break;
        }
    }

    auto colliderView = registry->view<TransformComponent, ColliderComponent>();
    for (auto entityId : colliderView) {
        auto transform = registry->getComponent<TransformComponent>(entityId);
        auto colliders = registry->getComponents<ColliderComponent>(entityId);

        if (!transform || colliders.empty())
            continue;

        bool isBoundary = registry->hasComponent<GameZoneColliderTag>(entityId);
        if (isBoundary) {
            _boundaryEntities.push_back(entityId);
            continue;
        }

        for (auto& collider : colliders) {
            if (collider->getType() == CollisionType::Solid ||
                collider->getType() == CollisionType::Push) {
                math::FRect hitbox = collider->getHitbox(
                    transform->getPosition(), transform->getScale(), transform->getRotation());
                _spatialGrid.insert(entityId, hitbox);
            }
        }
    }
}

void MovementSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;

    buildSpatialGrid(registry);

    auto view = registry->view<VelocityComponent, TransformComponent>();

    for (auto entityId : view) {
        auto velocityComp = registry->getComponent<VelocityComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        math::Vector2f velocity = velocityComp->getVelocity();
        math::Vector2f currentPos = transform->getPosition();
        math::Vector2f desiredPos = currentPos + velocity * deltaTime;

        auto colliders = registry->getComponents<ColliderComponent>(entityId);
        bool hasPushCollider = false;
        for (auto& collider : colliders) {
            if (collider->getType() == CollisionType::Push) {
                hasPushCollider = true;
            }
        }

        math::Vector2f finalPos = calculateSmoothSlidingPosition(
            registry, entityId, currentPos, desiredPos);
        transform->setPosition(finalPos);

        if (hasPushCollider) {
            handlePushCollision(registry, entityId, finalPos, deltaTime);
        }
    }
}

bool MovementSystem::checkCollisionWithBoundaries(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f newPos) {

    auto movingColliders = registry->getComponents<ColliderComponent>(entityId);
    if (movingColliders.empty()) {
        return true;
    }

    auto movingTransform = registry->getComponent<TransformComponent>(entityId);
    math::Vector2f movingScale = movingTransform->getScale();

    for (auto& movingCollider : movingColliders) {
        if (movingCollider->getType() != CollisionType::Solid) continue;
        math::OrientedRect movingOriented = movingCollider->
            getOrientedHitbox(newPos, movingScale, movingTransform->getRotation());

        for (auto boundaryEntityId : _boundaryEntities) {
            if (boundaryEntityId == entityId) continue;

            auto otherTransform = registry->getComponent<TransformComponent>(boundaryEntityId);
            auto otherColliders = registry->getComponents<ColliderComponent>(boundaryEntityId);

            if (!otherTransform || otherColliders.empty())
                continue;

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid &&
                    otherCollider->getType() != CollisionType::Push) continue;

                if (!shouldCollide(
                    registry, entityId, *movingCollider, boundaryEntityId)) {
                    continue;
                }

                math::Vector2f otherScale = otherTransform->getScale();
                math::OrientedRect otherOriented = otherCollider->
                    getOrientedHitbox(otherTransform->getPosition(), otherScale,
                    otherTransform->getRotation());

                if (movingOriented.intersects(otherOriented)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool MovementSystem::checkCollision(
    std::shared_ptr<Registry> registry,
    size_t entityId,
    math::Vector2f newPos) {

    if (!checkCollisionWithBoundaries(registry, entityId, newPos)) {
        return false;
    }

    auto movingColliders = registry->getComponents<ColliderComponent>(entityId);
    if (movingColliders.empty()) {
        return true;
    }

    auto movingTransform = registry->getComponent<TransformComponent>(entityId);
    math::Vector2f movingScale = movingTransform->getScale();

    for (auto& movingCollider : movingColliders) {
        if (movingCollider->getType() != CollisionType::Solid) continue;

        math::FRect movingHitbox = movingCollider->getHitbox(newPos, movingScale,
            movingTransform->getRotation());
        math::OrientedRect movingOriented = movingCollider->getOrientedHitbox(newPos,
            movingScale, movingTransform->getRotation());
        auto nearbyEntities = _spatialGrid.query(movingHitbox);

        for (auto otherEntityId : nearbyEntities) {
            if (otherEntityId == entityId) continue;

            auto otherTransform = registry->getComponent<TransformComponent>(otherEntityId);
            auto otherColliders = registry->getComponents<ColliderComponent>(otherEntityId);

            if (!otherTransform || otherColliders.empty())
                continue;

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid &&
                    otherCollider->getType() != CollisionType::Push) continue;

                if (!shouldCollide(
                    registry, entityId, *movingCollider, otherEntityId)) {
                    continue;
                }

                math::Vector2f otherScale = otherTransform->getScale();
                math::OrientedRect otherOriented = otherCollider->
                    getOrientedHitbox(otherTransform->getPosition(), otherScale,
                    otherTransform->getRotation());

                if (movingOriented.intersects(otherOriented)) {
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

    for (auto& pushCollider : pushColliders) {
        if (pushCollider->getType() != CollisionType::Push) continue;
        math::OrientedRect pushOriented = pushCollider->getOrientedHitbox(finalPos,
            entityScale, entityTransform->getRotation());

        auto allEntities = registry->view<TransformComponent, ColliderComponent>();
        for (auto otherEntityId : allEntities) {
            if (otherEntityId == entityId) continue;

            auto otherTransform = registry->getComponent<TransformComponent>(otherEntityId);
            auto otherColliders = registry->getComponents<ColliderComponent>(otherEntityId);

            if (!otherTransform || otherColliders.empty())
                continue;

            for (auto& otherCollider : otherColliders) {
                if (otherCollider->getType() == CollisionType::None) continue;

                if (!shouldCollide(
                    registry, entityId, *pushCollider, otherEntityId)) {
                    continue;
                }

                math::Vector2f otherScale = otherTransform->getScale();
                math::Vector2f currentOtherPos = otherTransform->getPosition();
                math::OrientedRect otherOriented = otherCollider->
                    getOrientedHitbox(currentOtherPos, otherScale,
                    otherTransform->getRotation());

                if (pushOriented.intersects(otherOriented)) {
                    auto pusherVelocity = registry->getComponent<VelocityComponent>(entityId);
                    math::Vector2f pushVelocity = pusherVelocity->getVelocity();
                    math::Vector2f pushAmount = pushVelocity * deltaTime;
                    math::Vector2f newOtherPos = currentOtherPos + pushAmount;

                    if (checkCollision(registry, otherEntityId, newOtherPos)) {
                        otherTransform->setPosition(newOtherPos);
                    } else {
                        registry->addComponent<DeathIntentComponent>(
                            otherEntityId, std::make_shared<DeathIntentComponent>());
                    }
                }
            }
        }
    }
}

bool MovementSystem::shouldCollide(
    std::shared_ptr<Registry> registry,
    size_t entityA,
    const ColliderComponent& colliderA,
    size_t entityB
) {
    const TagRegistry& tagRegistry = TagRegistry::getInstance();
    const CollisionRules& collisionRules = CollisionRules::getInstance();
    std::vector<std::string> tagsA = tagRegistry.getTags(registry, entityA);
    std::vector<std::string> tagsB = tagRegistry.getTags(registry, entityB);

    return collisionRules.canCollide(colliderA.getType(), tagsA, tagsB);
}

}
