/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyBrowserState
*/

#ifndef LOBBYBROWSERSTATE_HPP_
#define LOBBYBROWSERSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class LobbyBrowserState : public AGameState {
public:
    LobbyBrowserState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LobbyBrowserState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Lobby Browser"; }
};

}  // namespace gsm

#endif  // LOBBYBROWSERSTATE_HPP_
