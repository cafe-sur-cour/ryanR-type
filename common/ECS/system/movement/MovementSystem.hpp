/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementSystem
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../component/base/IComponent.hpp"
#include "../../component/temporary/MovementIntentComponent.hpp"
#include "../../component/permanent/TransformComponent.hpp"
#include "../../component/permanent/SpeedComponent.hpp"
#include "../../../constants.hpp"

namespace ecs {

class MovementSystem : public ASystem {
    public:
        MovementSystem();
        ~MovementSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) override;

    private:
        bool checkCollision(std::shared_ptr<ARegistry> registry, size_t entityId, math::Vector2f newPos);
        math::Vector2f calculateSmoothMovement(std::shared_ptr<ARegistry> registry, size_t entityId, math::Vector2f startPos, math::Vector2f desiredPos);
        math::Vector2f calculateSlidingMovement(std::shared_ptr<ARegistry> registry, size_t entityId, math::Vector2f basePos, math::Vector2f desiredPos);
        math::Vector2f calculateSmoothSlidingPosition(std::shared_ptr<ARegistry> registry, size_t entityId, math::Vector2f startPos, math::Vector2f desiredPos);
};

}  // namespace ecs

#endif /* !MOVEMENTSYSTEM_HPP_ */
