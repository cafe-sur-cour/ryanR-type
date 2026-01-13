/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HideLifetimeSystem
*/

#ifndef HIDELIFETIMESYSTEM_HPP_
#define HIDELIFETIMESYSTEM_HPP_

#include "../../common/systems/base/ASystem.hpp"

namespace ecs {

class HideLifetimeSystem : public ASystem {
    public:
        HideLifetimeSystem();
        ~HideLifetimeSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !HIDELIFETIMESYSTEM_HPP_ */
