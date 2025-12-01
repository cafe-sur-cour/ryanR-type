/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsState
*/

#pragma once

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"

namespace gsm {

class SettingsState : public AGameState {
public:
    SettingsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~SettingsState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

}  // namespace gsm
