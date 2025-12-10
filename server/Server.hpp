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
#include "../libs/Network/INetwork.hpp"
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
            std::vector<asio::ip::udp::endpoint> getConnectedClientEndpoints() const override;
            size_t getClientCount() const override;

            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> getEventQueue() override;
            bool hasEvents() const override;

            /* Received Packet Handling */
            void processIncomingPackets() override;
            bool processConnections(asio::ip::udp::endpoint endpoint) override;
            bool processDisconnections(uint8_t idClient) override;
            bool processEvents(uint8_t idClient) override;
            bool processEndOfGame(uint8_t idClient) override;

            /* Sent Packet Handling */
            bool connectionPacket(asio::ip::udp::endpoint endpoint);
            bool gameStatePacket();
            bool mapPacket(std::vector<uint64_t> mapData, const asio::ip::udp::endpoint &endpoint);
            bool canStartPacket();

            void setCurrentMap(const std::vector<uint64_t> &map);
            std::vector<uint64_t> getCurrentMap() const;
        private:
            void loadNetworkLibrary();
            void loadBufferLibrary();
            void loadPacketLibrary();
            DLLoader<createNetworkLib_t> _networloader;
            DLLoader<createBuffer_t> _bufferloader;
            DLLoader<createPacket_t> _packetloader;
            uint8_t _nextClientId;
            uint32_t _sequenceNumber;
            std::vector<std::tuple<uint8_t, asio::ip::udp::endpoint, std::string>> _clients;

            std::shared_ptr<ServerConfig> _config;
            std::shared_ptr<net::INetwork> _network;
            std::shared_ptr<IBuffer> _buffer;
            std::shared_ptr<pm::IPacketManager> _packet;
            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> _eventQueue;

            std::vector<uint64_t> _currentMap;
            std::shared_ptr<ResourceManager> _resourceManager;

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
            std::vector<uint64_t> convertVelocityComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
    };
} // namespace rserv = r-type server

#endif /* !SERVER_HPP_ */
