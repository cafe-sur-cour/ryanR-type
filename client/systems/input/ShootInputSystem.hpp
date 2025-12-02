/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootInputSystem
*/

#ifndef SHOOTINPUTSYSTEM_HPP_
    #define SHOOTINPUTSYSTEM_HPP_

#include <memory>
#include "../../../common/systems/base/ASystem.hpp"

namespace ecs {

class ShootInputSystem : public ASystem {
    public:
        ShootInputSystem();
        ~ShootInputSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
};

}

#endif /* !SHOOTINPUTSYSTEM_HPP_ */
