/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneViewSystem
*/

#ifndef GAMEZONEVIEWSYSTEM_HPP_
#define GAMEZONEVIEWSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include "../../../common/components/permanent/GameZoneComponent.hpp"
#include <memory>

namespace ecs {

class GameZoneViewSystem : public ASystem {
    public:
        GameZoneViewSystem();
        ~GameZoneViewSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

}  // namespace ecs

#endif /* !GAMEZONEVIEWSYSTEM_HPP_ */
