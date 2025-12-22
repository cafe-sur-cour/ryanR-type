/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IGameState
*/

#pragma once

#include <memory>
#include <vector>
#include "../systems/base/ISystem.hpp"

namespace gsm {

class IGameStateMachine;

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void enter() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void exit() = 0;
    virtual void addSystem(std::shared_ptr<ecs::ISystem> system) = 0;
    virtual void addSystem(const std::string& systemName) = 0;
    virtual std::vector<std::shared_ptr<ecs::ISystem>> getSystems() const = 0;
};

} // namespace gsm
