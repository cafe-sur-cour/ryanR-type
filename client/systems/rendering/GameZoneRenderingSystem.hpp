/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneRenderingSystem
*/

#ifndef GAMEZONERENDERINGSYSTEM_HPP_
#define GAMEZONERENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/components/permanent/GameZoneComponent.hpp"
#include <memory>

namespace ecs {

class GameZoneRenderingSystem : public ASystem {
    public:
        GameZoneRenderingSystem();
        ~GameZoneRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

}  // namespace ecs

#endif /* !GAMEZONERENDERINGSYSTEM_HPP_ */
