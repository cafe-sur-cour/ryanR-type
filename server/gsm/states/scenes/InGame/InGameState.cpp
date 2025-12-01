/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"

namespace gsm {

InGameState::InGameState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void InGameState::enter() {
    // TODO(anyone): Initialize server in-game state
}

void InGameState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server in-game state
}

void InGameState::exit() {
    // TODO(anyone): Cleanup server in-game state
}

}  // namespace gsm
