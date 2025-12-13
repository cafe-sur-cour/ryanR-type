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
    _gsm->requestStateChange(std::make_shared<LobbyState>(_gsm, _resourceManager));
}

}  // namespace gsm
