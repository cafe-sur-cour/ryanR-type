/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnSystem
*/

#ifndef SPAWNSYSTEM_HPP_
#define SPAWNSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../CollisionRules/CollisionRules.hpp"
#include "../../systems/interactions/TagRegistry.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"

namespace ecs {

class SpawnSystem : public ASystem {
    public:
        SpawnSystem();
        ~SpawnSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        bool isPositionFree(
            Entity newEntity,
            const math::Vector2f& position,
            const std::vector<std::shared_ptr<ColliderComponent>>& newColliders,
            std::shared_ptr<TransformComponent> newTransform,
            std::shared_ptr<Registry> registry
        );

        math::Vector2f findNearestFreePosition(
            Entity newEntity,
            const math::Vector2f& originalPosition,
            std::shared_ptr<Registry> registry,
            float stepSize = 10.0f
        );
};

}

#endif /* !SPAWNSYSTEM_HPP_ */
