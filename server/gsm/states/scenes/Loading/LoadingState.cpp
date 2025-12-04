/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#include "LoadingState.hpp"

namespace gsm {

LoadingState::LoadingState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LoadingState::enter() {
}


}  // namespace gsm
