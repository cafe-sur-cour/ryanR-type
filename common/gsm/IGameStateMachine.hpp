/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IGameStateMachine
*/

#pragma once

#include <memory>
#include <stack>

namespace gsm {

class IGameState;

class IGameStateMachine {
public:
    virtual ~IGameStateMachine() = default;

    virtual void changeState(std::shared_ptr<IGameState> newState) = 0;
    virtual void pushState(std::shared_ptr<IGameState> newState) = 0;
    virtual void popState() = 0;
    virtual void requestStateChange(std::shared_ptr<IGameState> newState) = 0;

    virtual void update(float deltaTime) = 0;
};

} // namespace gsm
