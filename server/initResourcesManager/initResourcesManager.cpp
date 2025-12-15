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
#include "../../common/components/permanent/NetworkIdComponent.hpp"
#include "../../common/debug.hpp"
#include "initResourcesManager.hpp"

std::shared_ptr<ResourceManager> initResourcesManager(std::shared_ptr<rserv::Server> server,
    std::shared_ptr<ecs::Registry> registry, std::shared_ptr<Parser> parser,
    std::shared_ptr<ecs::ISystemManager> systemsManager,
    std::shared_ptr<gsm::GameStateMachine> gameStateMachine,
    std::shared_ptr<ecs::IInputProvider> inputProvider) {
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
        auto weakServer = std::weak_ptr<rserv::Server>(server);
        auto weakRegistry = std::weak_ptr<ecs::Registry>(registry);
        entityPrefabManager->setOnEntityCreated(
            [weakServer, weakRegistry](ecs::Entity entity, const std::string& prefabName) {
                auto lockedServer = weakServer.lock();
                auto lockedRegistry = weakRegistry.lock();
                if (lockedServer && lockedRegistry && lockedServer->getNetwork() != nullptr
                    && lockedServer->getPacketManager() != nullptr) {
                    auto netIdComp = lockedRegistry->getComponent<ecs::NetworkIdComponent>
                        (entity);
                    if (netIdComp) {
                        size_t networkId = netIdComp->getNetworkId();
                        debug::Debug::printDebug(lockedServer->getConfig()->getIsDebug(),
                            "[SERVER] Sending spawn packet for entity " +
                                std::to_string(networkId) + " prefab '" + prefabName + "'",
                            debug::debugType::NETWORK,
                            debug::debugLevel::INFO);
                        std::vector<uint64_t> spawnData =
                            lockedServer->spawnPacket(networkId, prefabName);
                        std::vector<uint8_t> spawnPacketData =
                            lockedServer->getPacketManager()->pack(0,
                            lockedServer->getSequenceNumber(), constants::PACKET_SPAWN,
                                spawnData);
                        lockedServer->getNetwork()->
                            broadcast(lockedServer->getConnectedClientEndpoints(),
                                spawnPacketData);
                        lockedServer->incrementSequenceNumber();
                    }
                }
            }
        );

        registry->setOnEntityDestroyed(
            [weakServer, weakRegistry](ecs::Entity entity) {
                auto lockedServer = weakServer.lock();
                auto lockedRegistry = weakRegistry.lock();
                if (lockedServer && lockedRegistry && lockedServer->getNetwork() != nullptr
                    && lockedServer->getPacketManager() != nullptr) {
                    auto netIdComp = lockedRegistry->getComponent<ecs::NetworkIdComponent>
                        (entity);
                    if (netIdComp) {
                        size_t networkId = netIdComp->getNetworkId();
                        std::vector<uint64_t> deathData =
                            lockedServer->deathPacket(networkId);
                        std::vector<uint8_t> deathPacketData =
                            lockedServer->getPacketManager()->pack(0,
                            lockedServer->getSequenceNumber(), constants::PACKET_DEATH,
                                deathData);
                        lockedServer->getNetwork()->
                            broadcast(lockedServer->getConnectedClientEndpoints(),
                                deathPacketData);
                        lockedServer->incrementSequenceNumber();
                    }
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
