/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InfiniteState
*/

#include "InfiniteState.hpp"
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include "../../../../../common/ECS/entity/Entity.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../Settings/SettingsState.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../components/rendering/HitboxRenderComponent.hpp"
#include "../../../../systems/rendering/AnimationRenderingSystem.hpp"
#include "../../../../systems/rendering/HitboxRenderingSystem.hpp"
#include "../../../../systems/rendering/RectangleRenderingSystem.hpp"
#include "../../../../systems/rendering/TextRenderingSystem.hpp"
#include "../../../../systems/rendering/ParallaxRenderingSystem.hpp"
#include "../../../../systems/rendering/SpriteRenderingSystem.hpp"
#include "../../../../systems/rendering/HealthBarRenderingSystem.hpp"
#include "../../../../systems/input/MovementInputSystem.hpp"
#include "../../../../systems/input/ShootInputSystem.hpp"
#include "../../../../systems/audio/SoundSystem.hpp"
#include "../../../../systems/network/NetworkInterpolationSystem.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
#include "../../../../../common/CollisionRules/CollisionRules.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../../../common/components/tags/ObstacleTag.hpp"
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../systems/rendering/GameZoneViewSystem.hpp"
#include "../../../../systems/audio/MusicSystem.hpp"
#include "../../../../components/temporary/MusicIntentComponent.hpp"
#include "../../../../../common/systems/spawn/SpawnSystem.hpp"
#include "../../../../systems/map/MapGeneratorSystem.hpp"

namespace gsm {

InfiniteState::InfiniteState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    _registry = std::make_shared<ecs::Registry>();
    _prefabManager = std::make_shared<EntityPrefabManager>();
    this->_parser = nullptr;
}

void InfiniteState::enter() {
    _resourceManager->add<EntityPrefabManager>(_prefabManager);
    _resourceManager->add<ecs::Registry>(_registry);

    auto collisionData =
        ecs::CollisionRulesParser::parseFromFile("configs/rules/collision_rules.json");
    ecs::CollisionRules::initWithData(collisionData);
    auto existingParser = _resourceManager->get<Parser>();
    if (existingParser) {
        _parser = std::make_shared<Parser>(_prefabManager, ParsingType::CLIENT, _registry);
        _parser->parseAllEntities(constants::CONFIG_PATH);
        this->_parser->parseMapFromFile("configs/map/infinite.json");

        auto mapData = existingParser->getMapParser()->getMapJson();
        if (!mapData.is_null()) {
            _parser->getMapParser()->setMapJson(mapData);
            _parser->getMapParser()->generateMapEntities();
        }
    } else {
        _parser = std::make_shared<Parser>(_prefabManager, ParsingType::CLIENT, _registry);
        _parser->parseAllEntities(constants::CONFIG_PATH);
    }

    addSystem(std::make_shared<ecs::NetworkInterpolationSystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::MovementSystem>());
    addSystem(std::make_shared<ecs::MovementInputSystem>());
    addSystem(std::make_shared<ecs::InteractionSystem>());
    addSystem(std::make_shared<ecs::SoundSystem>());
    addSystem(std::make_shared<ecs::ShootInputSystem>());
    addSystem(std::make_shared<ecs::ShootingSystem>());
    addSystem(std::make_shared<ecs::LifetimeSystem>());
    addSystem(std::make_shared<ecs::HealthSystem>());
    addSystem(std::make_shared<ecs::OutOfBoundsSystem>());
    addSystem(std::make_shared<ecs::DeathSystem>());
    addSystem(std::make_shared<ecs::ScoreSystem>());
    addSystem(std::make_shared<ecs::GameZoneViewSystem>());
    addSystem(std::make_shared<ecs::MusicSystem>());
    addSystem(std::make_shared<ecs::TriggerSystem>());
    addSystem(std::make_shared<ecs::InteractionSystem>());
    addSystem(std::make_shared<ecs::ParallaxRenderingSystem>());
    addSystem(std::make_shared<ecs::SpriteRenderingSystem>());
    addSystem(std::make_shared<ecs::RectangleRenderingSystem>());
    addSystem(std::make_shared<ecs::AnimationRenderingSystem>());
    addSystem(std::make_shared<ecs::HitboxRenderingSystem>());
    addSystem(std::make_shared<ecs::HealthBarRenderingSystem>());
    addSystem(std::make_shared<ecs::TextRenderingSystem>());
    addSystem(std::make_shared<ecs::SpawnSystem>());
    addSystem(std::make_shared<ecs::MapGeneratorSystem>());

    auto audio = _resourceManager->get<gfx::IAudio>();

    ecs::Entity musicIntentEntity = _registry->createEntity();
    _registry->addComponent<ecs::MusicIntentComponent>(musicIntentEntity,
        std::make_shared<ecs::MusicIntentComponent>(ecs::PLAY, ""));

    ecs::Entity playerEntity = _prefabManager->createEntityFromPrefab(
        "player", _registry, ecs::EntityCreationContext::forServer());
    _registry->addComponent<ecs::LocalPlayerTag>(
        playerEntity, std::make_shared<ecs::LocalPlayerTag>());
    _registry->addComponent<ecs::HitboxRenderComponent>(
        playerEntity,
        std::make_shared<ecs::HitboxRenderComponent>());

    auto colliderView = _registry->view<ecs::ColliderComponent>();
    for (auto entityId : colliderView) {
        if (_registry->hasComponent<ecs::PlayerTag>(entityId)) continue;

        gfx::color_t color = {255, 255, 255, 255};
        if (_registry->hasComponent<ecs::ObstacleTag>(entityId)) {
            color = {255, 0, 0, 255};
        }

        _registry->addComponent<ecs::HitboxRenderComponent>(
            entityId,
            std::make_shared<ecs::HitboxRenderComponent>(color));
    }
}

void InfiniteState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
            if (auto stateMachine = _gsm.lock()) {
                stateMachine->requestStatePush(std::make_shared<SettingsState>(stateMachine,
                    _resourceManager));
                return;
            }
        }
    }

    _resourceManager->get<ecs::ISystemManager>()->updateAllSystems
        (_resourceManager, _registry, deltaTime);
}

void InfiniteState::exit() {
}

}  // namespace gsm
