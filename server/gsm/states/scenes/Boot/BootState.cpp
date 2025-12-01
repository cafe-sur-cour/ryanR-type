/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#include "BootState.hpp"

namespace gsm {

BootState::BootState(std::shared_ptr<IGameStateMachine> gsm)
    : AGameState(gsm) {
}

void BootState::enter() {
    // TODO(anyone): Initialize server boot state
}

void BootState::update(float deltaTime) {
    (void)deltaTime;
    // TODO(anyone): Update server boot state
}

void BootState::exit() {
    // TODO(anyone): Cleanup server boot state
}

}  // namespace gsm
