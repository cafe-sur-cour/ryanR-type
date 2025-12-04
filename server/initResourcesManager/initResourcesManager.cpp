/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initResourcesManager
*/

#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
#include "../../common/resourceManager/ResourceManager.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "ServerInputProvider.hpp"
#include "../ServerConfig.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/Parser/Parser.hpp"
#include "../../common/systems/systemManager/ASystemManager.hpp"
#include "../gsm/machine/GameStateMachine.hpp"
#include "initResourcesManager.hpp"

std::shared_ptr<ResourceManager> initResourcesManager(
    std::shared_ptr<rserv::Server> server,
    std::shared_ptr<ecs::Registry> registry,
    std::shared_ptr<Parser> parser,
    std::shared_ptr<ecs::ISystemManager> systemsManager,
    std::shared_ptr<gsm::GameStateMachine> gameStateMachine,
    std::shared_ptr<ecs::IInputProvider> inputProvider
) {
    std::shared_ptr<ResourceManager> resourceManager =
        std::make_shared<ResourceManager>();

    resourceManager->add<ecs::IInputProvider>(inputProvider);

    auto entityPrefabManager = std::make_shared<EntityPrefabManager>();
    resourceManager->add<EntityPrefabManager>(entityPrefabManager);

    resourceManager->add<rserv::Server>(server);
    resourceManager->add<rserv::ServerConfig>(server->getConfig());

    resourceManager->add<ecs::Registry>(registry);
    resourceManager->add<Parser>(parser);
    resourceManager->add<ecs::ISystemManager>(systemsManager);
    resourceManager->add<gsm::GameStateMachine>(gameStateMachine);

    return resourceManager;
}
