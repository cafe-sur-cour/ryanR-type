#include "DevState.hpp"
#include <memory>
#include <cmath>
#include <iostream>
#include "../../../../common/ECS/component/tags/PlayerTag.hpp"
#include "../../../../client/graphicals/IWindow.hpp"
#include "../../../../client/graphicals/IEvent.hpp"

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

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
}

void DevState::enter() {
    size_t entityId = 0;

    auto transform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(400.0f, 300.0f));
    auto playerTag = std::make_shared<ecs::PlayerTag>();

    _registry->addComponent(entityId, transform);
    _registry->addComponent(entityId, playerTag);
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
    _resourceManager->get<gfx::IWindow>()->clear();

    auto view = _registry->view<ecs::TransformComponent>();
    for (auto entityId : view) {
        auto transform =
            _registry->getComponent<ecs::TransformComponent>(entityId);
        math::Vector2f pos = transform->getPosition();
        _resourceManager->get<gfx::IWindow>()->drawRectangle(
            gfx::color_t{0, 255, 0},
            {pos.getX() - 25, pos.getY() - 25},
            {50, 50});
    }

    _resourceManager->get<gfx::IWindow>()->display();
}

void DevState::exit() {
}

}  // namespace gsm
