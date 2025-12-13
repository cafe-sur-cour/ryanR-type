/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkInterpolationSystem
*/

#ifndef NETWORKINTERPOLATIONSYSTEM_HPP_
#define NETWORKINTERPOLATIONSYSTEM_HPP_

#include "../../common/systems/base/ASystem.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../interpolation/NetworkStateComponent.hpp"
#include "../../common/components/permanent/TransformComponent.hpp"
#include "../../common/components/permanent/HealthComponent.hpp"

namespace ecs {

class NetworkInterpolationSystem : public ASystem {
    public:
        NetworkInterpolationSystem() = default;
        ~NetworkInterpolationSystem() override = default;

        void update(std::shared_ptr<ResourceManager> resourceManager,
                   std::shared_ptr<Registry> registry,
                   float deltaTime) override {
            (void)resourceManager;
            (void)deltaTime;
            for (Entity entity = 0; entity < registry->getMaxEntityId(); ++entity) {
                if (registry->hasComponent<NetworkStateComponent>(entity) &&
                    registry->hasComponent<TransformComponent>(entity)) {
                    auto networkState = registry->getComponent<NetworkStateComponent>(entity);
                    auto transform = registry->getComponent<TransformComponent>(entity);
                    if (networkState->hasTransform()) {
                        interpolateTransform(networkState, transform);
                    }
                }
                if (registry->hasComponent<NetworkStateComponent>(entity) &&
                    registry->hasComponent<HealthComponent>(entity)) {
                    auto networkState = registry->getComponent<NetworkStateComponent>(entity);
                    auto health = registry->getComponent<HealthComponent>(entity);
                    if (networkState->hasHealth()) {
                        const auto& currentHealth = networkState->getCurrentHealth();
                        health->setHealth(static_cast<float>(currentHealth.health));
                        health->setBaseHealth(static_cast<float>(currentHealth.baseHealth));
                    }
                }
            }
        }

    private:
        void interpolateTransform(std::shared_ptr<NetworkStateComponent> networkState,
                std::shared_ptr<TransformComponent> transform) {
            const auto& prev = networkState->getPreviousTransform();
            const auto& curr = networkState->getCurrentTransform();
            float t = networkState->getTransformInterpolationFactor();
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
};

} // namespace ecs

#endif /* !NETWORKINTERPOLATIONSYSTEM_HPP_ */
