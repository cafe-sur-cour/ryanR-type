/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PauseState
*/

#ifndef PAUSESTATE_HPP_
#define PAUSESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"

namespace gsm {

class PauseState : public AGameState {
public:
    PauseState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~PauseState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // PAUSESTATE_HPP_
