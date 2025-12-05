/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShutdownState
*/

#ifndef SERVER_SHUTDOWNSTATE_HPP_
#define SERVER_SHUTDOWNSTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class ShutdownState : public AGameState {
public:
    ShutdownState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~ShutdownState() override = default;

    void enter() override;
};

}  // namespace gsm

#endif  // SERVER_SHUTDOWNSTATE_HPP_
