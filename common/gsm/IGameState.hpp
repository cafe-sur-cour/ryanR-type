/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IGameState
*/

#pragma once

#include <memory>

namespace gsm {

class IGameStateMachine;

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void enter() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void exit() = 0;
};

} // namespace gsm
