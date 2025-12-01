/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#pragma once

#include "../../AGameState.hpp"

namespace gsm {

class BootState : public AGameState {
public:
    BootState(std::shared_ptr<IGameStateMachine> gsm);
    ~BootState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm
