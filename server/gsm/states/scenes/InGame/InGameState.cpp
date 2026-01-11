/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"
#include <memory>
#include <iostream>
#include "../../../machine/GameStateMachine.hpp"
#include "../../../../Lobby.hpp"
#include "../../../../../common/gsm/IGameState.hpp"
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/movement/IntentToVelocitySystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/systems/shooting/ChargedShotSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../../common/systems/scripting/ScriptingSystem.hpp"
#include "../../../../systems/input/ServerMovementInputSystem.hpp"
#include "../../../../systems/input/ServerShootInputSystem.hpp"
#include "../../../../systems/input/ServerForceInputSystem.hpp"
#include "../../../../systems/gameEnd/EndOfMapDetectionSystem.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/Parser/MapParser/MapHandler.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
#include "../../../../../common/systems/spawn/SpawnSystem.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/permanent/ScriptingComponent.hpp"
#include "../../../gsmStates.hpp"
#include "../GameEnd/GameEndState.hpp"
#include "../LevelComplete/LevelCompleteState.hpp"

namespace gsm {

InGameState::InGameState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void InGameState::enter() {
    auto parser = _resourceManager->get<Parser>();
    if (parser) {
        parser->parseAllEntities(constants::CONFIG_PATH);
    }

    if (!_resourceManager->has<MapHandler>()) {
        auto mapHandler = std::make_shared<MapHandler>();
        mapHandler->parseAllLevels(constants::MAPS_PATH);
        _resourceManager->add<MapHandler>(mapHandler);
    }

    auto mapHandler = _resourceManager->get<MapHandler>();
    if (mapHandler && mapHandler->hasMaps()) {
        auto mapParser = parser->getMapParser();
        if (mapParser) {
            mapParser->setMapJson(mapHandler->getCurrentMapJson());
            mapParser->generateMapEntities();
        }
    }

    auto collisionData = ecs::CollisionRulesParser::parseFromFile(
        constants::COLLISION_RULES_PATH
    );
    *(_resourceManager->get<gsm::GameStateType>()) = gsm::GameStateType::IN_GAME;
    ecs::CollisionRules::initWithData(collisionData);
    addSystem(std::make_shared<ecs::ServerMovementInputSystem>());
    addSystem(std::make_shared<ecs::ServerShootInputSystem>());
    addSystem(std::make_shared<ecs::ServerForceInputSystem>());
    addSystem(std::make_shared<ecs::ScriptingSystem>());
    addSystem(std::make_shared<ecs::IntentToVelocitySystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::TriggerSystem>());
    addSystem(std::make_shared<ecs::InteractionSystem>());
    addSystem(std::make_shared<ecs::MovementSystem>());
    addSystem(std::make_shared<ecs::ShootingSystem>());
    addSystem(std::make_shared<ecs::ChargedShotSystem>());
    addSystem(std::make_shared<ecs::LifetimeSystem>());
    addSystem(std::make_shared<ecs::HealthSystem>());
    addSystem(std::make_shared<ecs::OutOfBoundsSystem>());
    addSystem(std::make_shared<ecs::DeathSystem>());
    addSystem(std::make_shared<ecs::ScoreSystem>());
    addSystem(std::make_shared<ecs::SpawnSystem>());
    addSystem(std::make_shared<ecs::EndOfMapDetectionSystem>());
}

void InGameState::exit() {
    auto registry = _resourceManager->get<ecs::Registry>();
    if (registry) {
        auto scriptingView = registry->view<ecs::ScriptingComponent>();
        for (auto entityId : scriptingView) {
            auto scriptingComp = registry->getComponent<ecs::ScriptingComponent>(entityId);
            if (scriptingComp) {
                scriptingComp->clearLuaReferences();
            }
        }
    }

    auto systemManager = _resourceManager->get<ecs::ISystemManager>();
    for (auto& sys : _systems) {
        systemManager->removeSystem(sys);
    }
    _systems.clear();
}

void InGameState::update(float deltaTime) {
    AGameState::update(deltaTime);
    auto registry = _resourceManager->get<ecs::Registry>();
    if (_resourceManager->has<gsm::GameStateType>()) {
        gsm::GameStateType currentState = *(_resourceManager->get<gsm::GameStateType>());

        if (currentState == gsm::GameStateType::LEVEL_COMPLETE) {
            _resourceManager->get<rserv::Lobby>()->levelCompletePacket();

            if (auto gsmPtr = _gsm.lock()) {
                if (auto gsm = std::dynamic_pointer_cast<GameStateMachine>(gsmPtr)) {
                    gsm->requestStateChange(std::make_shared<gsm::LevelCompleteState>
                        (gsmPtr, _resourceManager));
                }
            }
        } else if (currentState == gsm::GameStateType::GAME_END) {
            bool isWin = false;
            auto players = registry->view<ecs::PlayerTag>();

            if (players.begin() != players.end())
                isWin = true;
            _resourceManager->get<rserv::Lobby>()->endGamePacket(isWin);

            if (auto gsmPtr = _gsm.lock()) {
                if (auto gsm = std::dynamic_pointer_cast<GameStateMachine>(gsmPtr)) {
                    gsm->requestStateChange(std::make_shared<gsm::GameEndState>
                        (gsmPtr, _resourceManager));
                }
            }
        }
    }
}

}  // namespace gsm
