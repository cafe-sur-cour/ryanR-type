/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#include "MainMenuState.hpp"
#include <memory>

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ecs::ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
}

void MainMenuState::enter() {
    // TODO(anyone): Initialize main menu state
}

void MainMenuState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update main menu state
}

void MainMenuState::exit() {
    // TODO(anyone): Cleanup main menu state
}

}  // namespace gsm
