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
#include "../../../../common/ECS/system/rendering/AnimationRenderingSystemy.hpp"

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
    auto animationRenderingSystem =
        std::make_shared<ecs::AnimationRenderingSystemy>();

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
    _systemManager->addSystem(_spriteRenderingSystem);
    _systemManager->addSystem(animationRenderingSystem);
}

void DevState::enter() {
    size_t entityId = 0;

    auto transform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(100.0f, 100.0f));

    transform->setScale(math::Vector2f(3.0f, 3.0f));
    auto playerTag = std::make_shared<ecs::PlayerTag>();
    auto controllableTag = std::make_shared<ecs::ControllableTag>();
    // auto sprite = std::make_shared<ecs::SpriteComponent>
    //     ("assets/sprites/sprite.png");

    auto animation = std::make_shared<ecs::AnimationComponent>
        ("assets/sprites/frog_spritesheet.png", 64.0f, 32.0f, 4);
    _registry->addComponent(entityId, transform);
    _registry->addComponent(entityId, playerTag);
    _registry->addComponent(entityId, controllableTag);
    // _registry->addComponent(entityId, sprite);
    _registry->addComponent(entityId, animation);
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
