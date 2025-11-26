#include "DevState.hpp"
#include <memory>
#include <cmath>
#include <iostream>
#include "../../../../common/ECS/component/tags/PlayerTag.hpp"
#include "../../../../client/graphicals/IWindow.hpp"
#include "../../../../client/graphicals/IEvent.hpp"
#include "../../../../common/ECS/component/permanent/SpriteComponent.hpp"
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

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
    _systemManager->addSystem(_spriteRenderingSystem);
}

void DevState::enter() {
    int entityId = 0;

    auto transform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(100.0f, 100.0f));

    transform->setScale(math::Vector2f(0.2f, 0.2f));
    auto playerTag = std::make_shared<ecs::PlayerTag>();
    auto sprite = std::make_shared<ecs::SpriteComponent>("assets/sprites/sprite.png");

    _registry->addComponent(entityId, transform);
    _registry->addComponent(entityId, playerTag);
    _registry->addComponent(entityId, sprite);
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
