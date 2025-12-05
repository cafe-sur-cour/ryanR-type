/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShutdownState
*/

#include "ShutdownState.hpp"
#include <memory>
namespace gsm {

ShutdownState::ShutdownState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void ShutdownState::enter() {
    // TODO(anyone): Initialize server shutdown state
}


}  // namespace gsm
