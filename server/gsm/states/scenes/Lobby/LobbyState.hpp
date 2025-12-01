/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#ifndef SERVER_LOBBYSTATE_HPP_
#define SERVER_LOBBYSTATE_HPP_

#include "../../AGameState.hpp"

namespace gsm {

class LobbyState : public AGameState {
public:
    LobbyState(std::shared_ptr<IGameStateMachine> gsm);
    ~LobbyState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
};

}  // namespace gsm

#endif  // SERVER_LOBBYSTATE_HPP_
