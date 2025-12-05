/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#ifndef SERVER_INGAMESTATE_HPP_
#define SERVER_INGAMESTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "systems/base/ISystem.hpp"

namespace gsm {

class InGameState : public AGameState {
public:
    InGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~InGameState() override = default;

    void enter() override;


    private:

};

}  // namespace gsm

#endif  // SERVER_INGAMESTATE_HPP_
