/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LifetimeSystem
*/

#ifndef LIFETIMESYSTEM_HPP_
#define LIFETIMESYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <memory>

namespace ecs {

class LifetimeSystem : public ASystem {
    public:
        LifetimeSystem();
        ~LifetimeSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !LIFETIMESYSTEM_HPP_ */
