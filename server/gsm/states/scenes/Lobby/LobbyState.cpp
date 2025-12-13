/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"
#include <memory>
#include "../Loading/LoadingState.hpp"
#include "../../../../../server/Server.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

LobbyState::LobbyState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LobbyState::enter() {
}

void LobbyState::update(float deltaTime) {
    (void)deltaTime;
    auto server = _resourceManager->get<rserv::Server>();
    if (server && server->allClientsReady()) {
        _gsm->requestStateChange(std::make_shared<LoadingState>(_gsm, _resourceManager));
    }
}


}  // namespace gsm
