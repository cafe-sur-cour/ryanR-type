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
#include "../Server.hpp"
#include "../Lobby.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/Parser/Parser.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "../gsm/machine/GameStateMachine.hpp"
#include "../../common/debug.hpp"
#include "../../common/constants.hpp"
#include "initResourcesManager.hpp"
#include "../gsm/gsmStates.hpp"

std::shared_ptr<ResourceManager> initResourcesManager(std::shared_ptr<rserv::Server> server,
    std::shared_ptr<rserv::Lobby> lobby) {
    std::shared_ptr<ResourceManager> resourceManager =
        std::make_shared<ResourceManager>();

    ecs::Entity nextEntityId = 1;
    if (server != nullptr) {
        nextEntityId = server->getNextEntityId();
    }

    std::shared_ptr<ecs::Registry> registry = std::make_shared<ecs::Registry>(nextEntityId);
    std::shared_ptr<ecs::ISystemManager> systemsManager = std::make_shared<ecs::SystemManager>();
    std::shared_ptr<gsm::GameStateMachine> gameStateMachine = std::make_shared<gsm::GameStateMachine>();
    std::shared_ptr<ecs::IInputProvider> inputProvider = std::make_shared<ecs::ServerInputProvider>();

    auto entityPrefabManager = std::make_shared<EntityPrefabManager>();
    std::shared_ptr<Parser> parser = std::make_shared<Parser>(
        entityPrefabManager,
        ParsingType::SERVER,
        registry
    );

    resourceManager->add<ecs::IInputProvider>(inputProvider);

    auto serverInputProvider = std::dynamic_pointer_cast
        <ecs::ServerInputProvider>(inputProvider);
    if (serverInputProvider) {
        resourceManager->add<ecs::ServerInputProvider>(serverInputProvider);
    }

    auto entityPrefabManagerPtr = parser->getPrefabManager();
    resourceManager->add<EntityPrefabManager>(entityPrefabManagerPtr);
    if (server != nullptr) {
        resourceManager->add<rserv::Server>(server);
        resourceManager->add<rserv::ServerConfig>(server->getConfig());
        resourceManager->add<rserv::Lobby>(lobby);
        auto weakServer = std::weak_ptr<rserv::Server>(server);
        auto weakLobby = std::weak_ptr<rserv::Lobby>(lobby);
        auto weakRegistry = std::weak_ptr<ecs::Registry>(registry);
        entityPrefabManager->setOnEntityCreated(
            [weakServer, weakLobby, weakRegistry](ecs::Entity entity, const std::string& prefabName) {
                auto lockedServer = weakServer.lock();
                auto lockedLobby = weakLobby.lock();
                auto lockedRegistry = weakRegistry.lock();
                if (lockedServer && lockedLobby && lockedRegistry && lockedServer->getNetwork() != nullptr
                    && lockedLobby->getPacketManager() != nullptr) {
                    size_t entityId = entity;
                    debug::Debug::printDebug(lockedServer->getConfig()->getIsDebug(),
                        "[SERVER] Sending spawn packet for entity " +
                            std::to_string(entityId) + " prefab '" + prefabName + "'",
                        debug::debugType::NETWORK,
                        debug::debugLevel::INFO);

                        std::vector<uint64_t> spawnData =
                            lockedLobby->spawnPacket(entityId, prefabName);
                        std::vector<uint8_t> spawnPacketData =
                            lockedLobby->getPacketManager()->pack(0,
                            lockedLobby->getSequenceNumber(), constants::PACKET_SPAWN,
                                spawnData);
                        lockedLobby->getNetwork()->
                            broadcast(lockedLobby->getConnectedClientEndpoints(),
                                spawnPacketData);
                        lockedLobby->incrementSequenceNumber();
                }
            }
        );

        registry->setOnEntityDestroyed(
            [weakServer, weakLobby, weakRegistry](ecs::Entity entity) {
                auto lockedServer = weakServer.lock();
                auto lockedLobby = weakLobby.lock();
                auto lockedRegistry = weakRegistry.lock();
                if (lockedServer && lockedLobby && lockedRegistry && lockedServer->getNetwork() != nullptr
                    && lockedServer->getPacketManager() != nullptr) {
                    size_t entityId = entity;
                    std::vector<uint64_t> deathData =
                        lockedLobby->deathPacket(entityId);
                    std::vector<uint8_t> deathPacketData =
                        lockedLobby->getPacketManager()->pack(0,
                        lockedLobby->getSequenceNumber(), constants::PACKET_DEATH,
                            deathData);
                    lockedLobby->getNetwork()->
                        broadcast(lockedLobby->getConnectedClientEndpoints(),
                            deathPacketData);
                    lockedLobby->incrementSequenceNumber();
                }
            }
        );
    }

    resourceManager->add<ecs::Registry>(registry);
    resourceManager->add<Parser>(parser);
    resourceManager->add<ecs::ISystemManager>(systemsManager);
    resourceManager->add<gsm::GameStateMachine>(gameStateMachine);
    resourceManager->add<gsm::GameStateType>(std::make_shared<gsm::GameStateType>(gsm::BOOT));
    return resourceManager;
}
