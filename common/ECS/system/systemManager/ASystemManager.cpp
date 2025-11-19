/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystemManager
*/

#include "ASystemManager.hpp"
#include <algorithm>

ASystemManager::ASystemManager() {
}

ASystemManager::~ASystemManager() {
}

void ASystemManager::updateAllSystems(float deltaTime) {
    for (auto& system : _systems)
        system->updateSystem(deltaTime);
}

void ASystemManager::addSystem(std::shared_ptr<ISystem> system) {
    _systems.push_back(system);
}

void ASystemManager::removeSystem(std::shared_ptr<ISystem> system) {
    _systems.erase(std::remove(_systems.begin(), _systems.end(), system), _systems.end());
}


