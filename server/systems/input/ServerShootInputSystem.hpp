/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerShootInputSystem
*/

#ifndef SERVERSHOOTINPUTSYSTEM_HPP_
#define SERVERSHOOTINPUTSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../initResourcesManager/ServerInputProvider.hpp"
#include <memory>
#include <map>

namespace ecs {

class ServerShootInputSystem : public ASystem {
    public:
        ServerShootInputSystem();
        ~ServerShootInputSystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;

    private:
        void updateShootIntent(std::shared_ptr<Registry> registry, Entity entityId);

        std::map<ecs::Entity, float> _cooldowns;
};

} // namespace ecs

#endif /* !SERVERSHOOTINPUTSYSTEM_HPP_ */
