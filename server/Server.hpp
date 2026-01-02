/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#endif

#include <queue>
#include <map>
#include <memory>
#include "IServer.hpp"
#include "ServerConfig.hpp"
#include "deltaTracker/ComponentDeltaTracker.hpp"
#include "deltaTracker/ComponentSerializer.hpp"
#include "../libs/Network/INetwork.hpp"
#include "../libs/Network/common.hpp"
#include "../libs/Buffer/IBuffer.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../common/constants.hpp"
#include "../common/InputMapping/InputAction.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "Signal.hpp"

namespace rserv {
    class Server : public IServer {
        public:
            Server(std::shared_ptr<ResourceManager> resourceManager);
            ~Server();

            void init() override;
            void start() override;
            void stop() override;

            void setConfig(std::shared_ptr<ServerConfig> config) override;
            std::shared_ptr<ServerConfig> getConfig() const override;
            uint16_t getPort() const override;
            void setPort(uint16_t port) override;

            int getState() const override;
            void setState(int state) override;

            operator int() const noexcept override;

            std::shared_ptr<net::INetwork> getNetwork() const override;
            void setNetwork(std::shared_ptr<net::INetwork> network) override;

            void onClientConnected(uint8_t idClient) override;
            void onClientDisconnected(uint8_t idClient) override;
            void onPacketReceived(uint8_t idClient, const pm::IPacketManager &packet) override;

            std::vector<uint8_t> getConnectedClients() const override;
            std::vector<std::shared_ptr<net::INetworkEndpoint>> getConnectedClientEndpoints() const override;
            size_t getClientCount() const override;

            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> getEventQueue() override;
            bool hasEvents() const override;

            /* Received Packet Handling */
            void processIncomingPackets() override;
            bool processConnections(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> client) override;
            bool processDisconnections(uint8_t idClient) override;
            bool processEvents(uint8_t idClient) override;
            bool processEndOfGame(uint8_t idClient) override;
            bool processWhoAmI(uint8_t idClient);

            /* Sent Packet Handling */
            bool connectionPacket(const net::INetworkEndpoint& endpoint);
            bool gameStatePacket();
            bool canStartPacket();
            bool endGamePacket(bool isWin);
            std::vector<uint64_t> spawnPacket(size_t entity, const std::string prefabName);
            std::vector<uint64_t> deathPacket(size_t entity);
            bool serverStatusPacket();
            std::vector<uint64_t> getCurrentMap() const;
            bool isGameStarted() const;
            bool allClientsReady() const;
            uint32_t getSequenceNumber() const;
            std::shared_ptr<pm::IPacketManager> getPacketManager() const;
            void incrementSequenceNumber();
            void setResourceManager(std::shared_ptr<ResourceManager> resourceManager);
            void clearEntityDeltaCache(uint8_t clientId, uint32_t entityId);
        private:
            void loadNetworkLibrary();
            void loadBufferLibrary();
            void loadPacketLibrary();
            DLLoader<createNetworkLib_t> _networloader;
            DLLoader<createBuffer_t> _bufferloader;
            DLLoader<createPacket_t> _packetloader;
            uint8_t _nextClientId;
            uint32_t _sequenceNumber;
            std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>> _clients;
            std::map<uint8_t, bool> _clientsReady;

            std::shared_ptr<ServerConfig> _config;
            std::shared_ptr<net::INetwork> _network;
            std::shared_ptr<IBuffer> _buffer;
            std::shared_ptr<pm::IPacketManager> _packet;
            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> _eventQueue;

            bool _gameStarted;
            std::shared_ptr<ResourceManager> _resourceManager;
            std::chrono::steady_clock::time_point _lastGameStateTime;

            ComponentDeltaTracker _deltaTracker;

            /* Functions to build game state packets */
            std::vector<std::function<std::vector<uint64_t>(std::shared_ptr<ecs::Registry>, ecs::Entity)>> _convertFunctions;
            std::vector<uint64_t> convertTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertTransformComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertSpeedComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertHealthComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertColliderComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertShootStatComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertScoreComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertAIMovementPatternComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertDamageComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertLifetimeComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertVelocityComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertAIMoverTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertAIShooterTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertControllableTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertEnemyProjectileTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertGameZoneColliderTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertMobTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertObstacleTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertPlayerProjectileTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertShooterTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertProjectilePassThroughTagComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertProjectilePrefabComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertGameZoneComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
    };
} // namespace rserv = r-type server

#endif /* !SERVER_HPP_ */
