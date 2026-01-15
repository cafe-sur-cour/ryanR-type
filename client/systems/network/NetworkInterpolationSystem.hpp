/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkInterpolationSystem
*/

#ifndef NETWORKINTERPOLATIONSYSTEM_HPP_
#define NETWORKINTERPOLATIONSYSTEM_HPP_

#include "../components/permanent/NetworkStateComponent.hpp"
#include "../../common/systems/base/ASystem.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/components/permanent/TransformComponent.hpp"
#include "../../common/components/permanent/HealthComponent.hpp"

namespace ecs {

class NetworkInterpolationSystem : public ASystem {
    public:
        NetworkInterpolationSystem() = default;
        ~NetworkInterpolationSystem() override = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void interpolateTransform(
            std::shared_ptr<NetworkStateComponent> networkState,
            std::shared_ptr<TransformComponent> transform
        );
        float getTransformInterpolationFactor(std::shared_ptr<NetworkStateComponent> networkState) const;
};

}  // namespace ecs

#endif /* !NETWORKINTERPOLATIONSYSTEM_HPP_ */
