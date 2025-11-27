#include "DevState.hpp"
#include <memory>
#include <cmath>
#include <iostream>
#include "../../../../common/ECS/component/tags/PlayerTag.hpp"
#include "../../../../common/ECS/component/tags/ControllableTag.hpp"
#include "../../../../client/graphicals/IWindow.hpp"
#include "../../../../client/graphicals/IEvent.hpp"
#include "../../../../common/ECS/component/permanent/SpriteComponent.hpp"
#include "../../../../common/ECS/component/permanent/AnimationComponent.hpp"
#include "../../../../common/ECS/component/permanent/TransformComponent.hpp"
#include "../../../../common/ECS/component/permanent/VelocityComponent.hpp"
#include "../../../../common/ECS/component/permanent/SpeedComponent.hpp"
#include "../../../../common/ECS/system/rendering/AnimationRenderingSystem.hpp"
#include "../../../../common/Prefab/PlayerPrefab/PlayerPrefab.hpp"

namespace gsm {

DevState::DevState(

    std::shared_ptr<IGameStateMachine> gsm,

    std::shared_ptr<ecs::ResourceManager> resourceManager)

    : AGameState(gsm), _resourceManager(resourceManager) {
    _registry = std::make_shared<ecs::ARegistry>();
    _systemManager = std::make_shared<ecs::ASystemManager>();
    _movementSystem = std::make_shared<ecs::MovementSystem>();
    _inputToVelocitySystem = std::make_shared<ecs::InputToVelocitySystem>();
    _inputSystem = std::make_shared<ecs::MovementInputSystem>();
    _spriteRenderingSystem = std::make_shared<ecs::SpriteRenderingSystem>();
    _prefabManager = std::make_shared<EntityPrefabManager>();
    auto animationRenderingSystem =
        std::make_shared<ecs::AnimationRenderingSystem>();

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
    _systemManager->addSystem(_spriteRenderingSystem);
    _systemManager->addSystem(animationRenderingSystem);
}

void DevState::enter() {
    auto playerPrefab = std::make_shared<PlayerPrefab>(
        100.0f,  // x
        100.0f,  // y
        3.0f,    // scale
        "assets/sprites/frog_spritesheet.png",  // animation path
        64.0f,   // frameWidth
        32.0f,   // frameHeight
        0.0f,    // startWidth
        96.0f,    // startHeight
        4);        // frameCount
    _prefabManager->registerPrefab("player", playerPrefab);
    size_t playerId = _prefabManager->createEntityFromPrefab
        ("player", _registry);
    (void)playerId;
}

void DevState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _systemManager->updateAllSystems(_resourceManager, _registry, deltaTime);
    _registry->removeAllComponentsWithState(ecs::ComponentState::Processed);
}

void DevState::render() {
    _resourceManager->get<gfx::IWindow>()->display();
    _resourceManager->get<gfx::IWindow>()->clear();
}

void DevState::exit() {
}

}  // namespace gsm
