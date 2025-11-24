/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystemManager
*/

#include <algorithm>
#include <memory>

#include "ASystemManager.hpp"
#include "../../context/AContext.hpp"

namespace ecs {

ASystemManager::ASystemManager() {
}

ASystemManager::~ASystemManager() {
}

void ASystemManager::updateAllSystems(
    std::shared_ptr<AContext> context, float deltaTime) {
    for (auto& system : _systems)
        system->updateSystem(context, deltaTime);
}

void ASystemManager::addSystem(std::shared_ptr<ISystem> system) {
    _systems.push_back(system);
}

void ASystemManager::removeSystem(std::shared_ptr<ISystem> system) {
    _systems.erase(
        std::remove(_systems.begin(), _systems.end(), system), _systems.end());
}

}  // namespace ecs
