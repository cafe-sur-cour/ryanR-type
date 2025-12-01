/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#ifndef SERVER_INGAMESTATE_HPP_
#define SERVER_INGAMESTATE_HPP_

#include "../../AGameState.hpp"

namespace gsm {

class InGameState : public AGameState {
public:
    InGameState(std::shared_ptr<IGameStateMachine> gsm);
    ~InGameState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm

#endif  // SERVER_INGAMESTATE_HPP_
