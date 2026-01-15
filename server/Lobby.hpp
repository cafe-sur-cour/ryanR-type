/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** Lobby
*/

#ifndef LOBBY_HPP_
#define LOBBY_HPP_
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
#include <mutex>

#include "LobbyStruct.hpp"
#include "ServerConfig.hpp"
#include "deltaTracker/ComponentDeltaTracker.hpp"
#include "deltaTracker/ComponentSerializer.hpp"
#include "../common/interfaces/INetwork.hpp"
#include "../common/interfaces/IBuffer.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../common/constants.hpp"
#include "../common/InputMapping/InputAction.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "Signal.hpp"

namespace rserv {

class Lobby {
        public:
            Lobby(std::shared_ptr<net::INetwork> network,
                std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>> lobbyPlayerInfo,
                std::string lobbyCode, bool debug, int64_t tps);
            ~Lobby();
            void stop();

            void startNetworkThread();
            void startGameThread();
            void networkLoop();
            void gameLoop();

            void setIsDebug(bool debug);
            bool getIsDebug() const;

            std::shared_ptr<ResourceManager> getResourceManager() const;

            const std::map<uint8_t, ecs::Entity>& getClientToEntity() const;

            std::vector<uint8_t> getConnectedClients() const;
            std::vector<std::tuple<uint8_t, std::string>> getConnectedClientDetails() const;
            std::vector<std::shared_ptr<net::INetworkEndpoint>> getConnectedClientEndpoints() const;
            size_t getClientCount() const;
            bool isRunning() const;
            void addClient(std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string> client);
            void removeClient(uint8_t clientId);
            void resetClientHeartbeats();
            void createPlayerEntityForClient(uint8_t clientId);
            void syncExistingEntitiesToClient(std::shared_ptr<net::INetworkEndpoint> clientEndpoint);
            std::string getLobbyCode() const;
            std::shared_ptr<net::INetwork> getNetwork() const;
            std::string getGameState() const;
            std::string getGameRules() const;

            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> getEventQueue();
            bool hasEvents() const;

            void enqueuePacket(std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>> packet);

            /* Received Packet Handling */
            void processIncomingPackets();
            bool processDisconnections(uint8_t idClient);
            bool processEvents(uint8_t idClient);
            bool processWhoAmI(uint8_t idClient);

            /* Sent Packet Handling */
            bool gameStatePacket();
            bool endGamePacket(bool isWin);
            std::vector<uint64_t> spawnPacket(size_t entity, const std::string prefabName);
            std::vector<uint64_t> deathPacket(size_t entity);
            bool serverStatusPacket();
            bool ackLeaveLobbyPacket(const net::INetworkEndpoint &endpoint, bool canDisconnect);

            bool levelCompletePacket();
            bool nextLevelPacket();
            bool gameRulesPacket();

            bool isGameStarted() const;
            bool allClientsReady() const;

            uint32_t getSequenceNumber() const;

            void setPacketManager(std::shared_ptr<pm::IPacketManager> packet);
            std::shared_ptr<pm::IPacketManager> getPacketManager() const;
            void incrementSequenceNumber();
            void setResourceManager(std::shared_ptr<ResourceManager> resourceManager);
            void clearEntityDeltaCache(uint8_t clientId, uint32_t entityId);
            void clearDeltaTrackerCaches();
            void createPlayerEntities();
            void processLobbyEvents();

        private:
            bool _isDebug;
            int64_t _tps;

            /* Network handling variable*/
            std::shared_ptr<net::INetwork> _network;
            std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>> _clients;
            std::string _lobbyCode;
            std::map<uint8_t, bool> _clientsReady;
            std::map<uint8_t, ecs::Entity> _clientToEntity;
            std::shared_ptr<pm::IPacketManager> _packet;
            uint32_t _sequenceNumber;
            std::shared_ptr<std::queue<std::tuple<uint8_t, constants::EventType, double>>> _eventQueue;

            /* Packet queue for incoming packets */
            std::queue<std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>>> _incomingPackets;
            std::mutex _packetMutex;
            mutable std::mutex _clientsMutex;

            /* ECS/Game handling variable */
            bool _gameStarted;
            bool _playerEntitiesCreated;
            std::shared_ptr<ResourceManager> _resourceManager;
            std::shared_ptr<gsm::GameStateMachine> _gsm;
            std::chrono::steady_clock::time_point _lastGameStateTime;
            float _statusUpdateTimer;

            /* Threading */
            std::atomic_bool _running;
            std::thread _networkThread;
            std::thread _gameThread;
            mutable std::mutex _eventMutex;


            ComponentDeltaTracker _deltaTracker;
            /* Functions to build game state packets */
            std::vector<std::function<std::vector<uint64_t>(std::shared_ptr<ecs::Registry>, ecs::Entity)>> _convertFunctions;

        protected:
            std::vector<uint64_t> convertTransformComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertHealthComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertScoreComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertAnimationStateComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);
            std::vector<uint64_t> convertChargedShotComponent(std::shared_ptr<ecs::Registry> registry, ecs::Entity i);

    };
} // namespace rserv = r-type server

#endif /* !LOBBY_HPP_ */
