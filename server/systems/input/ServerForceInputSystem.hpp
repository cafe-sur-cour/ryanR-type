/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ServerForceInputSystem
*/

#ifndef SERVERFORCEINPUTSYSTEM_HPP_
#define SERVERFORCEINPUTSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace ecs {

class ServerForceInputSystem : public ASystem {
    public:
        ServerForceInputSystem();
        ~ServerForceInputSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void callActivateOrDeactivateForce(
            std::shared_ptr<Registry> registry,
            Entity partId,
            Entity entityId
        );
};

}  // namespace ecs

#endif /* !SERVERFORCEINPUTSYSTEM_HPP_ */
