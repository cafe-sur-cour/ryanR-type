/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#ifndef SERVER_BOOTSTATE_HPP_
#define SERVER_BOOTSTATE_HPP_

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

#endif  // SERVER_BOOTSTATE_HPP_
