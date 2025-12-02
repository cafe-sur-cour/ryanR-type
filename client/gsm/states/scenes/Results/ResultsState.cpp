/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#include "ResultsState.hpp"
#include <memory>

namespace gsm {

ResultsState::ResultsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void ResultsState::enter() {
    // TODO(anyone): Initialize results state
}

void ResultsState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update results state
}

void ResultsState::exit() {
    // TODO(anyone): Cleanup results state
}

}  // namespace gsm
