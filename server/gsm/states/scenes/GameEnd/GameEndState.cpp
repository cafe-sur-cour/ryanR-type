/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndState
*/

#include "GameEndState.hpp"

namespace gsm {

GameEndState::GameEndState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void GameEndState::enter() {
    // TODO(anyone): Initialize server game end state
}

void GameEndState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server game end state
}

void GameEndState::exit() {
    // TODO(anyone): Cleanup server game end state
}

}  // namespace gsm
