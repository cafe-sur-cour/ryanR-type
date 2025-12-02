/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyBrowserState
*/

#ifndef LOBBYBROWSERSTATE_HPP_
#define LOBBYBROWSERSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"

namespace gsm {

class LobbyBrowserState : public AGameState {
public:
    LobbyBrowserState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager>urceManager);
    ~LobbyBrowserState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // LOBBYBROWSERSTATE_HPP_
