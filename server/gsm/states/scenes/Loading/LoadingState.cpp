/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#include "LoadingState.hpp"

namespace gsm {

LoadingState::LoadingState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void LoadingState::enter() {
    // TODO(anyone): Initialize server loading state
}

void LoadingState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server loading state
}

void LoadingState::exit() {
    // TODO(anyone): Cleanup server loading state
}

}  // namespace gsm
