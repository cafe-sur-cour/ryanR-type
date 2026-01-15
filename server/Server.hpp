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
#include <thread>
#include <chrono>
#include <mutex>

#include "LobbyStruct.hpp"
#include "Lobby.hpp"
#include "ServerConfig.hpp"
#include "deltaTracker/ComponentDeltaTracker.hpp"
#include "deltaTracker/ComponentSerializer.hpp"
#include "../common/interfaces/INetwork.hpp"
#include "../common/interfaces/IBuffer.hpp"
#include "../libs/Network/common.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../common/constants.hpp"
#include "../common/InputMapping/InputAction.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/systems/systemManager/ISystemManager.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "initResourcesManager/ServerInputProvider.hpp"
#include "Signal.hpp"
#include "http/HttpServer.hpp"

namespace rserv {
    class Server {
        public:
            Server();
            ~Server();

            void init();
            void start();
            void stop();

            void setConfig(std::shared_ptr<ServerConfig> config);
            std::shared_ptr<ServerConfig> getConfig() const;
            uint16_t getPort() const;
            void setPort(uint16_t port);

            int getState() const;
            void setState(int state);

            void initRessourceManager(std::shared_ptr<Lobby> lobby);
            operator int() const noexcept;

            std::shared_ptr<net::INetwork> getNetwork() const;
            void setNetwork(std::shared_ptr<net::INetwork> network);

            void onClientConnected(uint8_t idClient);
            void onClientDisconnected(uint8_t idClient);
            void onPacketReceived(uint8_t idClient, const pm::IPacketManager &packet);

            std::vector<uint8_t> getConnectedClients() const;
            std::vector<std::shared_ptr<net::INetworkEndpoint>> getConnectedClientEndpoints() const;
            size_t getClientCount() const;

            uint8_t findClientIdByEndpoint(const net::INetworkEndpoint &endpoint) const;

            ServerInfo getServerInfo() const;
            std::map<std::string, int> loadUserStats(const std::string& username) const;
            void saveUserBannedStatus(const std::string& username, bool banned) const;


            /* Received Packet Handling */
            void processIncomingPackets();
            bool processConnections(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> client);
            bool processDisconnections(uint8_t idClient);
            bool requestCode(const net::INetworkEndpoint &endpoint);
            bool processConnectToLobby(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> payload);
            bool processMasterStart(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> payload);
            bool processRegistration(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> client);
            bool processLogin(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> client);
            bool processLeaderboardRequest(std::shared_ptr<net::INetworkEndpoint> client);
            bool processProfileRequest(std::shared_ptr<net::INetworkEndpoint> client);
            bool processNewChatMessage(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> payload);
            bool processRequestGameRulesUpdate(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> payload);
            void cleanupClosedLobbies();
            void checkClientTimeouts();

            /* Sent Packet Handling */
            bool connectionPacket(const net::INetworkEndpoint& endpoint);
            bool canStartPacket(std::vector<std::shared_ptr<net::INetworkEndpoint>> endpoints);
            bool serverStatusPacket();
            bool sendCodeLobbyPacket(const net::INetworkEndpoint &endpoint);
            bool lobbyConnectValuePacket(const net::INetworkEndpoint &endpoint, bool canConnect);
            bool connectUserPacket(const net::INetworkEndpoint &endpoint, const std::string &username);
            bool leaderboardPacket(const net::INetworkEndpoint &endpoint);
            bool profilePacket(const net::INetworkEndpoint &endpoint);
            bool newChatMessagePacket(const net::INetworkEndpoint &endpoint, std::vector<uint8_t> message);
            bool forceLeavePacket(const net::INetworkEndpoint &endpoint, constants::ForceLeaveType leaveType);


            uint32_t getSequenceNumber() const;
            std::shared_ptr<pm::IPacketManager> getPacketManager() const;
            std::shared_ptr<pm::IPacketManager> createNewPacketManager();
            uint32_t getNextEntityId();
            void incrementSequenceNumber();

            std::string executeCommand(const std::string& command);
            std::string closeLobby(const std::string& lobbyId);
            std::string kickPlayer(const std::string& playerId);
            std::string banPlayer(const std::string& playerId);
            std::string unbanPlayer(const std::string& playerId);
            std::string toggleGodmod(const std::string& playerId);

        private:
            void loadNetworkLibrary();
            void loadBufferLibrary();
            void loadPacketLibrary();
            DLLoader<createNetworkLib_t> _networloader;
            DLLoader<createBuffer_t> _bufferloader;
            DLLoader<createPacket_t> _packetloader;

            std::shared_ptr<ServerConfig> _config;
            std::shared_ptr<net::INetwork> _network;
            std::shared_ptr<IBuffer> _buffer;
            std::shared_ptr<pm::IPacketManager> _packet;

            /* Network handling variables */
            uint8_t _nextClientId;
            uint32_t _sequenceNumber;
            uint32_t _nextEntityId;
            std::mutex _clientsMutex;

            /* Lobby handling variables */
            std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>> _clients;
            std::map<uint8_t, bool> _clientsReady;
            std::vector<std::shared_ptr<LobbyStruct>> _lobbyThreads;
            std::vector<std::shared_ptr<Lobby>> _lobbies;
            std::map<uint8_t, std::shared_ptr<Lobby>> _clientToLobby;
            std::unique_ptr<HttpServer> _httpServer;

            /* Healthcheck variables */
            std::map<uint8_t, std::chrono::steady_clock::time_point> _clientLastHeartbeat;

    };
}  // namespace rserv

#endif /* !SERVER_HPP_ */
