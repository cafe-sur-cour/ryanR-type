/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathSystem
*/

#ifndef DEATHSYSTEM_HPP_
#define DEATHSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <memory>

namespace ecs {

class DeathSystem : public ASystem {
    public:
        DeathSystem();
        ~DeathSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !DEATHSYSTEM_HPP_ */
