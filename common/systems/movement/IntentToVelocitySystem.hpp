/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IntentToVelocitySystem
*/

#ifndef INTENTTOVELOCITYSYSTEM_HPP_
#define INTENTTOVELOCITYSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class IntentToVelocitySystem : public ASystem {
    public:
        IntentToVelocitySystem();
        ~IntentToVelocitySystem() = default;

        void update(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<Registry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !INTENTTOVELOCITYSYSTEM_HPP_ */