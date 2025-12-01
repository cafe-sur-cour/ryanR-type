/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#ifndef SERVER_LOADINGSTATE_HPP_
#define SERVER_LOADINGSTATE_HPP_

#include "../../AGameState.hpp"

namespace gsm {

class LoadingState : public AGameState {
public:
    LoadingState(std::shared_ptr<IGameStateMachine> gsm);
    ~LoadingState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm

#endif  // SERVER_LOADINGSTATE_HPP_
