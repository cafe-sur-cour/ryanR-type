/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#ifndef RESULTSSTATE_HPP_
#define RESULTSSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"

namespace gsm {

class ResultsState : public AGameState {
public:
    ResultsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~ResultsState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // RESULTSSTATE_HPP_
