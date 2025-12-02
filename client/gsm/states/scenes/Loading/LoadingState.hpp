/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#ifndef LOADINGSTATE_HPP_
#define LOADINGSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"

namespace gsm {

class LoadingState : public AGameState {
public:
    LoadingState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LoadingState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // LOADINGSTATE_HPP_
