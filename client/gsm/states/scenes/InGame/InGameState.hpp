/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#ifndef INGAMESTATE_HPP_
#define INGAMESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"

namespace gsm {

class InGameState : public AGameState {
public:
    InGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~InGameState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // INGAMESTATE_HPP_
