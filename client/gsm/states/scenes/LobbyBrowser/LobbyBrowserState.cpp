/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyBrowserState
*/

#include "LobbyBrowserState.hpp"
#include <memory>

namespace gsm {

LobbyBrowserState::LobbyBrowserState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void LobbyBrowserState::enter() {
    // TODO(anyone): Initialize lobby browser state
}

void LobbyBrowserState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update lobby browser state
}

void LobbyBrowserState::exit() {
    // TODO(anyone): Cleanup lobby browser state
}

}  // namespace gsm
