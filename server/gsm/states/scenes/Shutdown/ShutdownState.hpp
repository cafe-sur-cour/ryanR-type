/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShutdownState
*/

#pragma once

#include "../../AGameState.hpp"

namespace gsm {

class ShutdownState : public AGameState {
public:
    ShutdownState(std::shared_ptr<IGameStateMachine> gsm);
    ~ShutdownState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm
