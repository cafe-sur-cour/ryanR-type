/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystemManager
*/

#include <algorithm>
#include <memory>

#include "ASystemManager.hpp"

namespace ecs {

ASystemManager::ASystemManager() {
    this->_systems = {};
}

ASystemManager::~ASystemManager() {
}

void ASystemManager::updateAllSystems(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry, float deltaTime) {
    for (auto& system : _systems)
        system->updateSystem(resourceManager, registry, deltaTime);
}

void ASystemManager::addSystem(std::shared_ptr<ISystem> system) {
    _systems.push_back(system);
}

void ASystemManager::removeSystem(std::shared_ptr<ISystem> system) {
    _systems.erase(
        std::remove(_systems.begin(), _systems.end(), system), _systems.end());
}

void ASystemManager::clearAllSystems() {
    _systems.clear();
}

}  // namespace ecs
