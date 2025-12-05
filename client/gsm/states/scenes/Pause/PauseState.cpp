/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PauseState
*/

#include "PauseState.hpp"
#include <memory>

namespace gsm {

PauseState::PauseState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void PauseState::enter() {
    // TODO(anyone): Initialize pause state
}

void PauseState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update pause state
}

void PauseState::exit() {
    // TODO(anyone): Cleanup pause state
}

}  // namespace gsm
