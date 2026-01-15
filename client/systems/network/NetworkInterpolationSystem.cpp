/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkInterpolationSystem
*/

#include "NetworkInterpolationSystem.hpp"
#include <memory>

namespace ecs {

void NetworkInterpolationSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

    auto entityView = registry->view<NetworkStateComponent, TransformComponent>();

    for (auto entity : entityView) {
        if (
            registry->hasComponent<NetworkStateComponent>(entity) &&
            registry->hasComponent<TransformComponent>(entity)
        ) {
            auto networkState = registry->getComponent<NetworkStateComponent>(entity);
            auto transform = registry->getComponent<TransformComponent>(entity);

            if (networkState->hasTransform())
                interpolateTransform(networkState, transform);
        }
    }
}

void NetworkInterpolationSystem::interpolateTransform(
    std::shared_ptr<NetworkStateComponent> networkState,
    std::shared_ptr<TransformComponent> transform
) {
    const auto& prev = networkState->getPreviousTransform();
    const auto& curr = networkState->getCurrentTransform();
    float t = getTransformInterpolationFactor(networkState);

    math::Vector2f interpolatedPos(
        prev.position.getX() + (curr.position.getX() - prev.position.getX()) * t,
        prev.position.getY() + (curr.position.getY() - prev.position.getY()) * t
    );
    float interpolatedRot = prev.rotation + (curr.rotation - prev.rotation) * t;
    float interpolatedScaleX = prev.scale.getX() + (curr.scale.getX() - prev.scale.getX()) * t;
    float interpolatedScaleY = prev.scale.getY() + (curr.scale.getY() - prev.scale.getY()) * t;
    math::Vector2f interpolatedScale(interpolatedScaleX, interpolatedScaleY);

    transform->setPosition(interpolatedPos);
    transform->setRotation(interpolatedRot);
    transform->setScale(interpolatedScale);
}

float NetworkInterpolationSystem::getTransformInterpolationFactor(
    std::shared_ptr<NetworkStateComponent> networkState
) const {
    if (!networkState->hasTransform())
        return 1.0f;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float>(
        now - networkState->getCurrentTransform().timestamp
    ).count();

    if (elapsed >= networkState->getInterpolationTime())
        return 1.0f;

    return elapsed / networkState->getInterpolationTime();
}

}  // namespace ecs
