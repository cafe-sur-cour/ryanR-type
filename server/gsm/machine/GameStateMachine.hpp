/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameStateMachine
*/

#pragma once

#include "AGameStateMachine.hpp"

namespace gsm {

class GameStateMachine : public AGameStateMachine {
public:
    GameStateMachine();
    ~GameStateMachine() override = default;

    void requestStateChange(std::shared_ptr<IGameState> newState) override;
};

}  // namespace gsm
