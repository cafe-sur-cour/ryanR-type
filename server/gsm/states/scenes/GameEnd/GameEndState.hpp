/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameEndState
*/

#ifndef SERVER_GAMEENDSTATE_HPP_
#define SERVER_GAMEENDSTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class GameEndState : public AGameState {
public:
    GameEndState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~GameEndState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    std::string getStateName() const override { return "Game Ended"; }
};

}  // namespace gsm

#endif  // SERVER_GAMEENDSTATE_HPP_
