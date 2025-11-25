#pragma once

#include "../base/AGameState.hpp"
#include "../../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../../common/ECS/system/systemManager/ASystemManager.hpp"
#include "../../../../common/ECS/system/movement/MovementSystem.hpp"
#include "../../../../common/ECS/system/input/MovementInputSystem.hpp"

namespace gsm {

class DevState : public AGameState {
public:
    DevState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~DevState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void render() override;
    void exit() override;

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
    std::shared_ptr<ecs::ARegistry> _registry;
    std::shared_ptr<ecs::ASystemManager> _systemManager;
    std::shared_ptr<ecs::MovementSystem> _movementSystem;
    std::shared_ptr<ecs::MovementInputSystem> _inputSystem;
};

} // namespace gsm
