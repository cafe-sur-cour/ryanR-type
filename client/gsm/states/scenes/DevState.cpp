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
#include "../../../../common/ECS/component/permanent/ColliderComponent.hpp"
#include "../../../../common/ECS/system/rendering/AnimationRenderingSystem.hpp"
#include "../../../../common/Prefab/PlayerPrefab/PlayerPrefab.hpp"
#include "../../../../common/ECS/component/tags/ObstacleTag.hpp"

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
        500.0f,  // x
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

    // Create a static wall entity
    size_t wallId = _registry->createEntity();
    auto wallTransform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(300.0f, 200.0f),
        0.0f,
        math::Vector2f(1.0f, 1.0f));

    auto wallCollider = std::make_shared<ecs::ColliderComponent>(
        math::Vector2f(0.0f, 0.0f),
        math::Vector2f(100.0f, 100.0f),
        ecs::CollisionType::Solid);

    _registry->addComponent(wallId, wallTransform);
    _registry->addComponent(wallId, wallCollider);
    _registry->addComponent(wallId, std::make_shared<ecs::ObstacleTag>());

    // Create a bouncing projectile
    size_t projectileId = _registry->createEntity();
    auto projectileTransform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(100.0f, 250.0f),
        0.0f,
        math::Vector2f(1.0f, 1.0f));

    auto projectileVelocity = std::make_shared<ecs::VelocityComponent>(
        math::Vector2f(200.0f, 0.0f));

    auto projectileCollider = std::make_shared<ecs::ColliderComponent>(
        math::Vector2f(0.0f, 0.0f),
        math::Vector2f(20.0f, 20.0f),
        ecs::CollisionType::Bounce);

    _registry->addComponent(projectileId, projectileTransform);
    _registry->addComponent(projectileId, projectileVelocity);
    _registry->addComponent(projectileId, projectileCollider);

    size_t projectileId2 = _registry->createEntity();
    auto projectileTransform2 = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(110.0f, 200.0f),
        0.0f,
        math::Vector2f(1.0f, 1.0f));

    auto projectileVelocity2 = std::make_shared<ecs::VelocityComponent>(
        math::Vector2f(150.0f / 2.0f, 75.0f / 2.0f));

    auto projectileCollider2 = std::make_shared<ecs::ColliderComponent>(
        math::Vector2f(0.0f, 0.0f),
        math::Vector2f(20.0f, 20.0f),
        ecs::CollisionType::Bounce);

    _registry->addComponent(projectileId2, projectileTransform2);
    _registry->addComponent(projectileId2, projectileVelocity2);
    _registry->addComponent(projectileId2, projectileCollider2);
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
    // Draw the player's hitbox
    auto view = _registry->view<
        ecs::PlayerTag, ecs::TransformComponent, ecs::ColliderComponent>();
    for (auto entityId : view) {
        auto transform =
            _registry->getComponent<ecs::TransformComponent>(entityId);
        auto colliders =
            _registry->getComponents<ecs::ColliderComponent>(entityId);
        if (!transform || colliders.empty()) continue;

        auto collider = colliders[0];
        math::FRect hitbox = collider->getHitbox(transform->getPosition());

        gfx::color_t red = {0, 0, 255};
        _resourceManager->get<gfx::IWindow>()->drawRectangle(
            red,
            {static_cast<size_t>(hitbox.getLeft()),
                static_cast<size_t>(hitbox.getTop())},
            {static_cast<size_t>(hitbox.getWidth()),
                static_cast<size_t>(hitbox.getHeight())});
        break;
    }

    // Draw static walls (entities with Transform and Collider but no PlayerTag)
    auto wallView =
        _registry->view<ecs::TransformComponent, ecs::ColliderComponent>();
    for (auto entityId : wallView) {
        if (_registry->hasComponent<ecs::PlayerTag>(entityId)) continue;

        auto transform =
            _registry->getComponent<ecs::TransformComponent>(entityId);
        auto colliders =
            _registry->getComponents<ecs::ColliderComponent>(entityId);
        if (!transform || colliders.empty()) continue;

        auto collider = colliders[0];
        math::FRect hitbox = collider->getHitbox(transform->getPosition());

        gfx::color_t color;
        if (collider->getType() == ecs::CollisionType::Bounce) {
            color = {255, 0, 0};  // Red for bouncing entities
        } else {
            color = {0, 255, 0};  // Green for solid entities
        }

        _resourceManager->get<gfx::IWindow>()->drawRectangle(
            color,
            {static_cast<size_t>(hitbox.getLeft()),
                static_cast<size_t>(hitbox.getTop())},
            {static_cast<size_t>(hitbox.getWidth()),
                static_cast<size_t>(hitbox.getHeight())});
    }

    _resourceManager->get<gfx::IWindow>()->display();
    _resourceManager->get<gfx::IWindow>()->clear();
}

void DevState::exit() {
}

}  // namespace gsm
