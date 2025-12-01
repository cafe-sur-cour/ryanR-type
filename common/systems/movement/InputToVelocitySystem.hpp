/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** VelocitySystem
*/

#ifndef VELOCITYSYSTEM_HPP_
#define VELOCITYSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class InputToVelocitySystem : public ASystem {
    public:
        InputToVelocitySystem();
        ~InputToVelocitySystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !VELOCITYSYSTEM_HPP_ */