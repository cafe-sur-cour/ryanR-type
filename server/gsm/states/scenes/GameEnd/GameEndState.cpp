/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndState
*/

#include "GameEndState.hpp"
#include <memory>
namespace gsm {

GameEndState::GameEndState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void GameEndState::enter() {
    // TODO(anyone): Initialize server game end state
}


}  // namespace gsm
