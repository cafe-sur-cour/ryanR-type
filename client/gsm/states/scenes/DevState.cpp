#include "DevState.hpp"
#include "../../../../common/ECS/component/permanent/TransformComponent.hpp"
#include "../../../../common/ECS/component/permanent/SpeedComponent.hpp"
#include "../../../../common/ECS/component/temporary/MovementIntentComponent.hpp"
#include "../../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../../common/ECS/system/systemManager/ASystemManager.hpp"
#include "../../../../client/graphicals/IWindow.hpp"
#include "../../../../client/graphicals/IEvent.hpp"
#include <cmath>
#include <iostream>

namespace gsm {

DevState::DevState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
    _registry = std::make_shared<ecs::ARegistry>();
    _systemManager = std::make_shared<ecs::ASystemManager>();
    _movementSystem = std::make_shared<ecs::MovementSystem>();
    _inputSystem = std::make_shared<ecs::MovementInputSystem>();

    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
}

void DevState::enter() {
    int entityId = 0;

    auto transform = std::make_shared<ecs::TransformComponent>(math::Vector2f(400.0f, 300.0f));
    auto speed = std::make_shared<ecs::SpeedComponent>(200.0f);
    auto movementIntent = std::make_shared<ecs::MovementIntentComponent>();

    _registry->addComponent(entityId, transform);
    _registry->addComponent(entityId, speed);
    _registry->addComponent(entityId, movementIntent);
}

void DevState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents({0, 0});
    if (eventResult == gfx::IEvent::CLOSE) {
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
        auto transform = _registry->getComponent<ecs::TransformComponent>(entityId);
        math::Vector2f pos = transform->getPosition();
        _resourceManager->get<gfx::IWindow>()->drawRectangle(
            gfx::color_t{0, 255, 0}, {pos.getX() - 25, pos.getY() - 25}, {50, 50});
    }

    _resourceManager->get<gfx::IWindow>()->display();
}

void DevState::exit() {
}

}  // namespace gsm
