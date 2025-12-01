/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndState
*/

#pragma once

#include "../../AGameState.hpp"

namespace gsm {

class GameEndState : public AGameState {
public:
    GameEndState(std::shared_ptr<IGameStateMachine> gsm);
    ~GameEndState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm
