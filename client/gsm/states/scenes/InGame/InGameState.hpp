/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#ifndef INGAMESTATE_HPP_
#define INGAMESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"

namespace gsm {

class InGameState : public AGameState {
public:
    InGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~InGameState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // INGAMESTATE_HPP_
