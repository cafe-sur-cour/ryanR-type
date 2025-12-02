/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyState
*/

#ifndef LOBBYSTATE_HPP_
#define LOBBYSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"

namespace gsm {

class LobbyState : public AGameState {
public:
    LobbyState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LobbyState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // LOBBYSTATE_HPP_
