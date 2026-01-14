/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndState
*/

#include "GameEndState.hpp"
#include <memory>
#include <iostream>

namespace gsm {

GameEndState::GameEndState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void GameEndState::update(float deltaTime) {
    (void)deltaTime;
}

}  // namespace gsm
