/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PauseState
*/

#ifndef PAUSESTATE_HPP_
#define PAUSESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class PauseState : public AGameState {
public:
    PauseState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~PauseState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Paused"; }
};

}  // namespace gsm

#endif  // PAUSESTATE_HPP_
