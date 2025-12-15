/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#include "BootState.hpp"
#include <memory>
#include <iostream>
#include "../Lobby/LobbyState.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/constants.hpp"

namespace gsm {

BootState::BootState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void BootState::enter() {
    _resourceManager->get<Parser>()->parseAllEntities(constants::CONFIG_PATH);
    if (auto stateMachine = _gsm.lock()) {
        stateMachine->requestStateChange(std::make_shared<LobbyState>(stateMachine, _resourceManager));
    }
}

}  // namespace gsm
