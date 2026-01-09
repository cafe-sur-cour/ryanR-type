/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneStopSystem
*/

#ifndef GAMEZONESTOPSYSTEM_HPP_
#define GAMEZONESTOPSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class GameZoneStopSystem : public ASystem {
    public:
        GameZoneStopSystem();
        ~GameZoneStopSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !GAMEZONESTOPSYSTEM_HPP_ */