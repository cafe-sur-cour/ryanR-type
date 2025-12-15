/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AGameState
*/

#include "AGameState.hpp"
#include <vector>
#include <memory>
#include <stdexcept>
#include "../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/Error/ClientError.hpp"

namespace gsm {

AGameState::AGameState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager) {
    _gsm = gsm;
    _resourceManager = resourceManager;
    this->_systems = std::vector<std::shared_ptr<ecs::ISystem>>();
}

AGameState::~AGameState() {
    if (this->_resourceManager != nullptr) {
        this->_systems.clear();
    }
    while (!_systems.empty()) {
        _systems.pop_back();
    }
}

void AGameState::enter() {
}

void AGameState::update(float deltaTime) {
    auto systemsManager = _resourceManager->get<ecs::ISystemManager>();
    auto registry = _resourceManager->get<ecs::Registry>();
    if (systemsManager && registry) {
        systemsManager->updateAllSystems(_resourceManager, registry, deltaTime);
    }
}

void AGameState::exit() {
}

void AGameState::addSystem(std::shared_ptr<ecs::ISystem> system) {
    if (_resourceManager->has<ecs::ISystemManager>())
        _resourceManager->get<ecs::ISystemManager>()->addSystem(system);
    else
        throw err::ClientError("ISystemManager not found in ResourceManager",
            err::ClientError::UNKNOWN);
    _systems.push_back(system);
}

std::vector<std::shared_ptr<ecs::ISystem>> AGameState::getSystems() const {
    return _systems;
}

}  // namespace gsm
