/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#ifndef SERVER_LOADINGSTATE_HPP_
#define SERVER_LOADINGSTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class LoadingState : public AGameState {
public:
    LoadingState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LoadingState() override = default;

    void enter() override;
    std::string getStateName() const override { return "Loading"; }
};

}  // namespace gsm

#endif  // SERVER_LOADINGSTATE_HPP_
