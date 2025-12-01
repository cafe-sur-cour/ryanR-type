/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BootState
*/

#ifndef BOOTSTATE_HPP_
#define BOOTSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"

namespace gsm {

class BootState : public AGameState {
public:
    BootState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~BootState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

}  // namespace gsm

#endif  // BOOTSTATE_HPP_
