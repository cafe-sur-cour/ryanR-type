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
#include "../../../../Server.hpp"
#include "../../../../../common/gsm/IGameState.hpp"
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../systems/input/ServerMovementInputSystem.hpp"
#include "../../../../systems/input/ServerShootInputSystem.hpp"
#include "../../../../systems/gameEnd/EndOfMapDetectionSystem.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/systems/SystemNames.hpp"
#include "../../../../../common/systems/SystemNames.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/systems/ai/AIMovementSystem.hpp"
#include "../../../../../common/systems/ai/AIShootingSystem.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
#include "../../../../../common/systems/spawn/SpawnSystem.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../gsmStates.hpp"
#include "../GameEnd/GameEndState.hpp"

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

    auto collisionData = ecs::CollisionRulesParser::parseFromFile(
        "configs/rules/collision_rules.json"
    );
    *(_resourceManager->get<gsm::GameStateType>()) = gsm::IN_GAME;
    ecs::CollisionRules::initWithData(collisionData);
    addSystem(std::make_shared<ecs::ServerMovementInputSystem>());
    addSystem(std::make_shared<ecs::ServerShootInputSystem>());
    addSystem(std::make_shared<ecs::AIMovementSystem>());
    addSystem(std::make_shared<ecs::AIShootingSystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::MovementSystem>());
    addSystem(std::make_shared<ecs::ShootingSystem>());
    addSystem(std::make_shared<ecs::LifetimeSystem>());
    addSystem(std::make_shared<ecs::HealthSystem>());
    addSystem(std::make_shared<ecs::OutOfBoundsSystem>());
    addSystem(ecs::systems::DEATH_SYSTEM);
    addSystem(std::make_shared<ecs::ScoreSystem>());
    addSystem(std::make_shared<ecs::TriggerSystem>());
    addSystem(std::make_shared<ecs::InteractionSystem>());
    addSystem(std::make_shared<ecs::SpawnSystem>());
    addSystem(std::make_shared<ecs::EndOfMapDetectionSystem>());
}

void InGameState::update(float deltaTime) {
    AGameState::update(deltaTime);
    auto registry = _resourceManager->get<ecs::Registry>();
    if (_resourceManager->has<gsm::GameStateType>()) {
        gsm::GameStateType currentState = *(_resourceManager->get<gsm::GameStateType>());
        if (currentState == gsm::GAME_END) {
            bool isWin = false;
            auto players = registry->view<ecs::PlayerTag>();

            if (players.begin() != players.end())
                isWin = true;
            _resourceManager->get<rserv::Server>()->endGamePacket(isWin);

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
