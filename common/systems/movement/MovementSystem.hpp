/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementSystem
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../components/temporary/MovementIntentComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../CollisionRules/CollisionRules.hpp"
#include "../../systems/interactions/TagRegistry.hpp"
#include "../../SpatialGrid/SpatialGrid.hpp"

namespace ecs {

class MovementSystem : public ASystem {
    public:
        MovementSystem();
        ~MovementSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void buildSpatialGrid(std::shared_ptr<Registry> registry);

        bool checkCollision(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f newPos
        );
        math::Vector2f calculateSmoothMovement(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f startPos,
            math::Vector2f desiredPos
        );
        math::Vector2f calculateSlidingMovement(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f basePos,
            math::Vector2f desiredPos
        );
        math::Vector2f calculateSmoothSlidingPosition(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f startPos,
            math::Vector2f desiredPos
        );
        void handlePushCollision(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f finalPos,
            float deltaTime
        );
        bool shouldCollide(
            std::shared_ptr<Registry> registry,
            size_t entityA,
            const ColliderComponent& colliderA,
            size_t entityB
        );
        bool checkCollisionWithBoundaries(
            std::shared_ptr<Registry> registry,
            size_t entityId,
            math::Vector2f newPos
        );

        SpatialGrid _spatialGrid;
        std::vector<Entity> _boundaryEntities;
};

}

#endif /* !MOVEMENTSYSTEM_HPP_ */
