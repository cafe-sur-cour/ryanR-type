/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#include "ASystem.hpp"

namespace ecs {

ASystem::ASystem() {
}

void ASystem::updateSystem(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) {
    if (resourceManager && registry) {
        update(resourceManager, registry, deltaTime);
    }
}

}  // namespace ecs
