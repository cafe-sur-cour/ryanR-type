/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShutdownState
*/

#include "ShutdownState.hpp"

namespace gsm {

ShutdownState::ShutdownState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void ShutdownState::enter() {
    // TODO(anyone): Initialize server shutdown state
}

void ShutdownState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server shutdown state
}

void ShutdownState::exit() {
    // TODO(anyone): Cleanup server shutdown state
}

}  // namespace gsm
