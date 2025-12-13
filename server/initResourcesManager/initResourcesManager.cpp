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
#include <vector>
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

    auto serverInputProvider = std::dynamic_pointer_cast
        <ecs::ServerInputProvider>(inputProvider);
    if (serverInputProvider) {
        resourceManager->add<ecs::ServerInputProvider>(serverInputProvider);
    }

    auto entityPrefabManager = parser->getPrefabManager();
    resourceManager->add<EntityPrefabManager>(entityPrefabManager);

    if (server != nullptr) {
        resourceManager->add<rserv::Server>(server);
        resourceManager->add<rserv::ServerConfig>(server->getConfig());
        entityPrefabManager->setOnEntityCreated(
            [server](ecs::Entity entity, const std::string& prefabName) {
                if (server->getNetwork() != nullptr && server->getPacketManager() != nullptr) {
                    std::vector<uint64_t> spawnData = server->spawnPacket(entity, prefabName);
                    std::vector<uint8_t> spawnPacketData = server->getPacketManager()->pack(0,
                        server->getSequenceNumber(), constants::PACKET_SPAWN, spawnData);
                    server->getNetwork()->
                        broadcast(server->getConnectedClientEndpoints(), spawnPacketData);
                    server->incrementSequenceNumber();
                }
            }
        );
        registry->setOnEntityDestroyed(
            [server](ecs::Entity entity) {
                if (server->getNetwork() != nullptr && server->getPacketManager() != nullptr) {
                    std::vector<uint64_t> deathData = server->deathPacket(entity);
                    std::vector<uint8_t> deathPacketData = server->getPacketManager()->pack(0,
                        server->getSequenceNumber(), constants::PACKET_DEATH, deathData);
                    server->getNetwork()->
                        broadcast(server->getConnectedClientEndpoints(), deathPacketData);
                    server->incrementSequenceNumber();
                }
            }
        );
    }

    resourceManager->add<ecs::Registry>(registry);
    resourceManager->add<Parser>(parser);
    resourceManager->add<ecs::ISystemManager>(systemsManager);
    resourceManager->add<gsm::GameStateMachine>(gameStateMachine);

    return resourceManager;
}
