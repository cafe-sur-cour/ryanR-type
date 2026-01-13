/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InfiniteState
*/

#ifndef SERVER_INFINITESTATE_HPP_
#define SERVER_INFINITESTATE_HPP_

#include "../../AGameState.hpp"
#include <memory>
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class InfiniteState : public AGameState {
    public:
        InfiniteState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~InfiniteState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "Infinite Mode"; }
};

}  // namespace gsm

#endif  // SERVER_INFINITESTATE_HPP_
