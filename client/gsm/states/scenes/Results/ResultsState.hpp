/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#ifndef RESULTSSTATE_HPP_
#define RESULTSSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class ResultsState : public AGameState {
    public:
        ResultsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager, bool isWin);
        ~ResultsState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;

    private:
        bool _isWin;
        std::shared_ptr<ecs::Registry> _registry;
};

}  // namespace gsm

#endif  // RESULTSSTATE_HPP_
