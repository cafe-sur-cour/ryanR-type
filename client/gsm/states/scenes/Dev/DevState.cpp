#include "DevState.hpp"
#include <memory>
#include <cmath>
#include <iostream>
#include "../../../../../common/ECS/entity/Entity.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/tags/ControllableTag.hpp"
#include "../../../../../libs/Multimedia/IWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../../libs/Multimedia/IAudio.hpp"
#include "../../../../components/rendering/SpriteComponent.hpp"
#include "../../../../components/rendering/AnimationComponent.hpp"
#include "../../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../../systems/rendering/AnimationRenderingSystem.hpp"
#include "../../../../components/rendering/HitboxRenderComponent.hpp"
#include "../../../../components/rendering/RectangleRenderComponent.hpp"
#include "../../../../systems/rendering/HitboxRenderingSystem.hpp"
#include "../../../../systems/rendering/RectangleRenderingSystem.hpp"
#include "../../../../../common/Prefab/PlayerPrefab/PlayerPrefab.hpp"
#include "../../../../../common/components/tags/ObstacleTag.hpp"

namespace gsm {

DevState::DevState(

    std::shared_ptr<IGameStateMachine> gsm,

    std::shared_ptr<ecs::ResourceManager> resourceManager)

    : AGameState(gsm), _resourceManager(resourceManager) {
    _registry = std::make_shared<ecs::Registry>();
    _systemManager = std::make_shared<ecs::ASystemManager>();
    _movementSystem = std::make_shared<ecs::MovementSystem>();
    _inputToVelocitySystem = std::make_shared<ecs::InputToVelocitySystem>();
    _inputSystem = std::make_shared<ecs::MovementInputSystem>();
    _spriteRenderingSystem = std::make_shared<ecs::SpriteRenderingSystem>();
    _prefabManager = std::make_shared<EntityPrefabManager>();
    auto animationRenderingSystem =
        std::make_shared<ecs::AnimationRenderingSystem>();
    auto hitboxRenderingSystem =
        std::make_shared<ecs::HitboxRenderingSystem>();
    auto rectangleRenderingSystem =
        std::make_shared<ecs::RectangleRenderingSystem>();

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
    _systemManager->addSystem(_spriteRenderingSystem);
    _systemManager->addSystem(animationRenderingSystem);
    _systemManager->addSystem(hitboxRenderingSystem);
    _systemManager->addSystem(rectangleRenderingSystem);
}

void DevState::enter() {
    auto audio = _resourceManager->get<gfx::IAudio>();
    if (audio) {
        audio->playMusic("musics/hava-nagila.ogg", true);
        audio->setMusicVolume(50.0f);
    }

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
    ecs::Entity playerId = _prefabManager->createEntityFromPrefab
        ("player", _registry);

    auto playerHitboxRender = std::make_shared<ecs::HitboxRenderComponent>(
        gfx::color_t{0, 0, 255}, 2.0f);
    _registry->addComponent(playerId, playerHitboxRender);

    // Create a static wall entity
    ecs::Entity wallId = _registry->createEntity();
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

    auto wallHitboxRender = std::make_shared<ecs::HitboxRenderComponent>(
        gfx::color_t{0, 255, 0}, 2.0f);
    _registry->addComponent(wallId, wallHitboxRender);

    auto wallRectangleRender = std::make_shared<ecs::RectangleRenderComponent>(
        gfx::color_t{0, 255, 0}, 100.0f, 100.0f);
    _registry->addComponent(wallId, wallRectangleRender);

    // Create a bouncing projectile
    ecs::Entity projectileId = _registry->createEntity();
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

    auto projectileHitboxRender = std::make_shared<ecs::HitboxRenderComponent>(
        gfx::color_t{255, 0, 0}, 2.0f);
    _registry->addComponent(projectileId, projectileHitboxRender);

    auto projectileRectangleRender =
        std::make_shared<ecs::RectangleRenderComponent>(
        gfx::color_t{255, 0, 0}, 20.0f, 20.0f);
    _registry->addComponent(projectileId, projectileRectangleRender);

    ecs::Entity projectileId2 = _registry->createEntity();
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

    auto projectileHitboxRender2 = std::make_shared<ecs::HitboxRenderComponent>(
        gfx::color_t{255, 0, 0}, 2.0f);
    _registry->addComponent(projectileId2, projectileHitboxRender2);

    auto projectileRectangleRender2 =
        std::make_shared<ecs::RectangleRenderComponent>(
        gfx::color_t{255, 0, 0}, 20.0f, 20.0f);
    _registry->addComponent(projectileId2, projectileRectangleRender2);
}

void DevState::update(float deltaTime) {
    auto event = _resourceManager->get<gfx::IEvent>();
    auto eventResult = event->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    bool isSpacePressed = event->isKeyPressed(gfx::EventType::SPACE);
    if (isSpacePressed && !_wasSpacePressed) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        if (audio) {
            audio->playSound("sounds/coin-sound.wav", 50.0f);
        }
    }
    _wasSpacePressed = isSpacePressed;

    _systemManager->updateAllSystems(_resourceManager, _registry, deltaTime);
    _registry->removeAllComponentsWithState(ecs::ComponentState::Processed);
}

void DevState::exit() {
}

}  // namespace gsm
