#include "AGameState.hpp"
#include <stdexcept>
#include <memory>
#include <vector>
#include <iostream>
#include "../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../common/systems/SystemLoader.hpp"
#include "../../../../common/Error/ClientError.hpp"

namespace gsm {

AGameState::AGameState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager) {
    _gsm = gsm;
    _resourceManager = resourceManager;
    _systemLoader = _resourceManager->get<ecs::SystemLoader>();
    this->_systems = {};
}

void AGameState::enter() {
}

void AGameState::update(float deltaTime) {
    (void)deltaTime;
}

void AGameState::exit() {
}

void AGameState::addSystem(std::shared_ptr<ecs::ISystem> system) {
    if (_resourceManager->has<ecs::ISystemManager>())
        _resourceManager->get<ecs::ISystemManager>()->addSystem(system);
    else
        throw err::ClientError("ISystemManager not found in ResourceManager",
            err::ClientError::NOT_INITIALIZED);
    _systems.push_back(system);
}

void AGameState::addSystem(const std::string& systemName) {
    auto system = _systemLoader->loadSystem(systemName);
    if (system) {
        addSystem(system);
    } else {
        std::cerr << "Failed to load system: " << systemName << std::endl;
    }
}

std::vector<std::shared_ptr<ecs::ISystem>> AGameState::getSystems() const {
    return _systems;
}

}  // namespace gsm
