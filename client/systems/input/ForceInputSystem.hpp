/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ForceInputSystem
*/

#ifndef FORCEINPUTSYSTEM_HPP_
#define FORCEINPUTSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace ecs {

class ForceInputSystem : public ASystem {
    public:
        ForceInputSystem();
        ~ForceInputSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        bool isPlayerAlive(
            std::shared_ptr<Registry> registry,
            Entity entityId
        ) const;
        float _lastForceTime;
};

}  // namespace ecs

#endif /* !FORCEINPUTSYSTEM_HPP_ */
