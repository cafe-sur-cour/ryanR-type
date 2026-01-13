/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#ifndef SERVER_BOOTSTATE_HPP_
#define SERVER_BOOTSTATE_HPP_

#include "../../AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"

namespace gsm {

class BootState : public AGameState {
public:
    BootState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~BootState() override = default;

    void enter() override;
    std::string getStateName() const override { return "Booting"; }
};

}  // namespace gsm

#endif  // SERVER_BOOTSTATE_HPP_
