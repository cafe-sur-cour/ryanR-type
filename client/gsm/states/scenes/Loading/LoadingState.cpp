/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#include "LoadingState.hpp"
#include <memory>

namespace gsm {

LoadingState::LoadingState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void LoadingState::enter() {
    // TODO(anyone): Initialize loading state
}

void LoadingState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update loading state
}

void LoadingState::exit() {
    // TODO(anyone): Cleanup loading state
}

}  // namespace gsm
