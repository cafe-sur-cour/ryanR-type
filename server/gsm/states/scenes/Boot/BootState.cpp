/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#include "BootState.hpp"
#include "../Lobby/LobbyState.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include <iostream>

namespace gsm {

BootState::BootState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void BootState::enter() {
    _gsm->requestStateChange(std::make_shared<LobbyState>(_gsm, _resourceManager));
}

}  // namespace gsm
