/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"
#include <memory>

namespace gsm {

InGameState::InGameState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void InGameState::enter() {
    // TODO(anyone): Initialize in-game state
}

void InGameState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update in-game state
}

void InGameState::exit() {
    // TODO(anyone): Cleanup in-game state
}

}  // namespace gsm
