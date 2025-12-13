/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#ifndef SERVER_LOBBYSTATE_HPP_
#define SERVER_LOBBYSTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class LobbyState : public AGameState {
public:
    LobbyState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LobbyState() override = default;

    void enter() override;
    void update(float deltaTime) override;
};

}  // namespace gsm

#endif  // SERVER_LOBBYSTATE_HPP_
