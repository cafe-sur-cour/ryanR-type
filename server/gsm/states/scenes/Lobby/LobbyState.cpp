/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"

namespace gsm {

LobbyState::LobbyState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void LobbyState::enter() {
    // TODO(anyone): Initialize server lobby state
}

void LobbyState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server lobby state
}

void LobbyState::exit() {
    // TODO(anyone): Cleanup server lobby state
}

}  // namespace gsm
