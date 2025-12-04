/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"
#include <memory>

namespace gsm {

SettingsState::SettingsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void SettingsState::enter() {
    // TODO(anyone): Initialize settings state
}

void SettingsState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update settings state
}

void SettingsState::exit() {
    // TODO(anyone): Cleanup settings state
}

}  // namespace gsm
