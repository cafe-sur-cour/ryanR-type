/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"
#include <memory>

namespace gsm {

LobbyState::LobbyState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void LobbyState::enter() {
    // TODO(anyone): Initialize lobby state
}

void LobbyState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update lobby state
}

void LobbyState::exit() {
    // TODO(anyone): Cleanup lobby state
}

}  // namespace gsm
