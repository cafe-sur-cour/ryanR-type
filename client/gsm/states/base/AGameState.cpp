#include "AGameState.hpp"
#include <stdexcept>
#include "../../../../common/systems/systemManager/ISystemManager.hpp"

namespace gsm {

AGameState::AGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager) {
    _gsm = gsm;
    _resourceManager = resourceManager;
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
        throw std::runtime_error("AGameState::addSystem: ISystemManager not found in ResourceManager");
    _systems.push_back(system);
}

std::vector<std::shared_ptr<ecs::ISystem>> AGameState::getSystems() const {
    return _systems;
}

}  // namespace gsm
