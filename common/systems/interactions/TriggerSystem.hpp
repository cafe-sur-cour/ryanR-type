/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerSystem
*/

#ifndef TRIGGERSYSTEM_HPP_
#define TRIGGERSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../components/base/IComponent.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../CollisionRules/CollisionRules.hpp"
#include "TagRegistry.hpp"

namespace ecs {

class TriggerSystem : public ASystem {
    public:
        TriggerSystem();
        ~TriggerSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        bool checkCollision(
            const TransformComponent& transformA,
            const ColliderComponent& colliderA,
            const TransformComponent& transformB,
            const ColliderComponent& colliderB
        );

        bool shouldCollide(
            std::shared_ptr<Registry> registry,
            size_t entityA,
            const ColliderComponent& colliderA,
            size_t entityB
        );
};

}  // namespace ecs

#endif /* !TRIGGERSYSTEM_HPP_ */
