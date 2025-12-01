/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#include "ASystem.hpp"
#include <memory>
namespace ecs {

ASystem::ASystem() {
}

void ASystem::updateSystem(std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry, float deltaTime) {
    if (resourceManager && registry) {
        update(resourceManager, registry, deltaTime);
    }
}

}  // namespace ecs
