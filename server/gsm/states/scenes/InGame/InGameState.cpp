/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"
#include <memory>
#include <iostream>
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../systems/input/ServerMovementInputSystem.hpp"
#include "../../../../systems/input/ServerShootInputSystem.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/systems/ai/AIMovementSystem.hpp"
#include "../../../../../common/systems/ai/AIShootingSystem.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
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

    auto collisionData =
        ecs::CollisionRulesParser::parseFromFile("configs/rules/collision_rules.json");
    ecs::CollisionRules::initWithData(collisionData);
    addSystem(std::make_shared<ecs::AIMovementSystem>());
    addSystem(std::make_shared<ecs::AIShootingSystem>());
    addSystem(std::make_shared<ecs::ServerMovementInputSystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::MovementSystem>());
    addSystem(std::make_shared<ecs::ServerShootInputSystem>());
    addSystem(std::make_shared<ecs::ShootingSystem>());
    addSystem(std::make_shared<ecs::LifetimeSystem>());
    addSystem(std::make_shared<ecs::HealthSystem>());
    addSystem(std::make_shared<ecs::DeathSystem>());
    addSystem(std::make_shared<ecs::ScoreSystem>());
    addSystem(std::make_shared<ecs::TriggerSystem>());
    addSystem(std::make_shared<ecs::InteractionSystem>());


    auto registry = _resourceManager->get<ecs::Registry>();
    if (_resourceManager->has<EntityPrefabManager>()) {
        auto prefabManager = _resourceManager->get<EntityPrefabManager>();
        prefabManager->createEntityFromPrefab("player",
            _resourceManager->get<ecs::Registry>(), ecs::EntityCreationContext::forServer());
    } else {
        throw std::runtime_error("EntityPrefabManager not found in ResourceManager");
    }
}

}  // namespace gsm
