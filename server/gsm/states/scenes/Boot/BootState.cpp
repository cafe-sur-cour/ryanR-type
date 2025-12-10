/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#include "BootState.hpp"
#include <memory>
#include "../Lobby/LobbyState.hpp"
#include "../InGame/InGameState.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

BootState::BootState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void BootState::enter() {
    _gsm->requestStateChange(std::make_shared<InGameState>(_gsm, _resourceManager));
}

}  // namespace gsm
