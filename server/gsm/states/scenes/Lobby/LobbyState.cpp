/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"
#include <memory>

namespace gsm {

LobbyState::LobbyState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LobbyState::enter() {
    // TODO(anyone): Initialize server lobby state
}


}  // namespace gsm
