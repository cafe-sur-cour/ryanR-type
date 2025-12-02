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
#include "../../../../components/temporary/SoundIntentComponent.hpp"
#include "../../../../systems/rendering/AnimationRenderingSystem.hpp"
#include "../../../../components/rendering/HitboxRenderComponent.hpp"
#include "../../../../components/rendering/RectangleRenderComponent.hpp"
#include "../../../../systems/rendering/HitboxRenderingSystem.hpp"
#include "../../../../systems/rendering/RectangleRenderingSystem.hpp"
#include "../../../../../common/components/tags/ObstacleTag.hpp"
#include "../../../../../common/components/tags/ShooterTag.hpp"
#include "../../../../systems/input/ShootInputSystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/Parser/Parser.hpp"

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
    _soundSystem = std::make_shared<ecs::SoundSystem>();
    _prefabManager = std::make_shared<EntityPrefabManager>();
    auto animationRenderingSystem =
        std::make_shared<ecs::AnimationRenderingSystem>();
    auto hitboxRenderingSystem =
        std::make_shared<ecs::HitboxRenderingSystem>();
    auto rectangleRenderingSystem =
        std::make_shared<ecs::RectangleRenderingSystem>();
    auto shootInputSystem = std::make_shared<ecs::ShootInputSystem>();
    auto shootingSystem = std::make_shared<ecs::ShootingSystem>();

    _systemManager->addSystem(_inputToVelocitySystem);
    _systemManager->addSystem(_movementSystem);
    _systemManager->addSystem(_inputSystem);
    _systemManager->addSystem(_soundSystem);
    _systemManager->addSystem(_spriteRenderingSystem);
    _systemManager->addSystem(animationRenderingSystem);
    _systemManager->addSystem(hitboxRenderingSystem);
    _systemManager->addSystem(rectangleRenderingSystem);
    _systemManager->addSystem(shootInputSystem);
    _systemManager->addSystem(shootingSystem);

    _parser = std::make_shared<Parser>(_prefabManager, ParsingType::CLIENT);
    _parser->parseAllEntities(constants::CONFIG_PATH);
}

void DevState::enter() {
    auto audio = _resourceManager->get<gfx::IAudio>();
    if (audio) {
        audio->playMusic("musics/hava-nagila.ogg", true);
        audio->setMusicVolume(50.0f);
    }

    _prefabManager->createEntityFromPrefab("player", _registry);
}

void DevState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _systemManager->updateAllSystems(_resourceManager, _registry, deltaTime);
}

void DevState::exit() {
}

}  // namespace gsm
