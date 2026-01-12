/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelCompleteState
*/

#ifndef SERVER_LEVELCOMPLETESTATE_HPP_
#define SERVER_LEVELCOMPLETESTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include <vector>

namespace gsm {

class LevelCompleteState : public AGameState {
public:
    LevelCompleteState(std::shared_ptr<IGameStateMachine> gsm,
        std::shared_ptr<ResourceManager> resourceManager);
    ~LevelCompleteState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    std::string getStateName() const override { return "Level Complete"; }

private:
    float _transitionTimer;
    std::vector<int> _savedPlayerScores;
    static constexpr float TRANSITION_DELAY = 2.0f;
};

}  // namespace gsm

#endif  // SERVER_LEVELCOMPLETESTATE_HPP_
