/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#include "BootState.hpp"
#include <memory>

namespace gsm {

BootState::BootState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ecs::ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void BootState::enter() {
    // TODO(anyone): Initialize boot state
}

void BootState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update boot state
}

void BootState::exit() {
    // TODO(anyone): Cleanup boot state
}

}  // namespace gsm
