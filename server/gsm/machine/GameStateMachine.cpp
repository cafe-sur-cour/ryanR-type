/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameStateMachine
*/

#include "GameStateMachine.hpp"

namespace gsm {

GameStateMachine::GameStateMachine() : AGameStateMachine() {
}

void GameStateMachine::requestStateChange(std::shared_ptr<IGameState> newState) {
    changeState(newState);
}

}  // namespace gsm
