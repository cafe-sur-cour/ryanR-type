/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#include "LobbyState.hpp"
#include <memory>
#include "../Loading/LoadingState.hpp"
#include "../../../../../server/Lobby.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../gsmStates.hpp"

namespace gsm {

LobbyState::LobbyState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LobbyState::enter() {
}

void LobbyState::update(float deltaTime) {
    (void)deltaTime;
    *(_resourceManager->get<gsm::GameStateType>()) = gsm::GameStateType::LOBBY;
    auto lobby = _resourceManager->get<rserv::Lobby>();
    if (lobby && lobby->isGameStarted()) {
        if (auto stateMachine = _gsm.lock()) {
            stateMachine->requestStateChange(std::make_shared<LoadingState>(stateMachine,
                _resourceManager));
        }
    }
}

}  // namespace gsm
