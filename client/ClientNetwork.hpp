/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/


#ifndef CLIENTNETWORK_HPP_
#define CLIENTNETWORK_HPP_

#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <chrono>

#include "../common/DLLoader/DLLoader.hpp"
#include "../common/DLLoader/LoaderType.hpp"
#include "../common/interfaces/INetwork.hpp"
#include "../common/constants.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include "../common/gsm/IGameStateMachine.hpp"
#include "gsm/states/scenes/MainMenu/MainMenuState.hpp"

namespace ecs {
    class Registry;
    using Entity = std::size_t;
}

struct NetworkEvent {
    constants::EventType eventType;
    double depth;
};

class ClientNetwork {
    public:
        ClientNetwork();
        ~ClientNetwork();

        void init();
        void start();
        void stop();
        void connect();

        uint16_t getPort() const;
        void setPort(int port);

        std::string getIp() const;
        void setIp(const std::string &ip);
        std::shared_ptr<net::INetwork> getNetwork() const;

        void setDebugMode(bool isDebug);
        bool isDebugMode() const;

        void loadNetworkLibrary();
        void loadBufferLibrary();
        void loadPacketLibrary();

        void sendConnectionData(std::vector<uint8_t> packet);

        std::string getName() const;
        void setName(const std::string &name);

        uint8_t getIdClient() const;
        void setIdClient(uint8_t idClient);

        std::string getLobbyCode() const;
        void setLobbyCode(std::string lobbyCode);

        net::ConnectionState getConnectionState() const;

        /* Packet Handling */
        void eventPacket(const constants::EventType &eventType, double depth);
        void disconnectionPacket();
        void connectionPacket();
        void sendWhoAmI();
        void requestCode();
        void sendLobbyConnection(std::string lobbyCode);
        void sendMasterStartGame();
        void sendRegisterPacket(const std::string &username, const std::string &password);
        void sendLoginPacket(const std::string &username, const std::string &password);
        void sendRequestLeaderboardPacket();
        void sendRequestProfilePacket();
        void sendMessageToServer(const std::string &message);
        void sendRequestGameRulesUpdate(uint8_t ruleType, uint8_t value);
        void sendDisconnectFromLobby();

        const std::vector<std::pair<std::string, std::string>>& getLeaderboardData() const;
        bool isLeaderboardDataUpdated() const;
        void clearLeaderboardDataUpdateFlag();

        const std::vector<std::string>& getProfileData() const;
        bool isProfileDataUpdated() const;
        void clearProfileDataUpdateFlag();
        const std::vector<std::pair<std::string, std::string>>& getLastMessages() const;

        void addToEventQueue(const NetworkEvent &event);

        void clearEntitiesAndMappings();

        bool isConnected() const;
        bool isReady() const;

        size_t getConnectedClients() const;
        size_t getReadyClients() const;
        uint8_t getClientId() const;
        bool getClientReadyStatus() const;

        bool isConnectedToLobby() const;
        bool isLobbyMaster() const;

        std::atomic<bool> _isConnected;
        std::atomic<bool> _ready;
        std::atomic<bool> _isConnectedToLobby;
        std::atomic<bool> _isLobbyMaster;

        std::atomic<size_t> _connectedClients;
        std::atomic<size_t> _readyClients;
        std::atomic<uint8_t> _clientId;
        std::atomic<bool> _clientReadyStatus;
        std::atomic<bool> _shouldDisconnect;

        std::chrono::steady_clock::time_point _lastLeaveLobbyTime;

        void setResourceManager(std::shared_ptr<ResourceManager> resourceManager);
        void setGameStateMachine(std::shared_ptr<gsm::IGameStateMachine> gsm);
        std::shared_ptr<gsm::IGameStateMachine> getGameStateMachine() const;

        void redoServerEndpoint();

    protected:
        std::pair<int, std::chrono::steady_clock::time_point> tryConnection(const int maxRetries, std::chrono::steady_clock::time_point lastRetryTime);
        void handlePacketType(uint8_t type);

    private:
        typedef void (ClientNetwork::*PacketHandler)();
        PacketHandler _packetHandlers[constants::MAX_INDEX_PACKET_TYPE];

        void handleNoOp();
        void handleConnectionAcceptation();
        void handleBatchedGameState();
        void handleEndGame();
        void handleCanStart();
        void handleEntitySpawn();
        void handleEntityDeath();
        void handleWhoAmI();
        void handleServerStatus();
        void handleCode();
        void handleLevelComplete();
        void handleNextLevel();
        void handleLobbyConnectValue();
        void handleConnectUser();
        void handleLeaderboard();
        void handleProfile();
        void handleRegisterFail();
        void handleBroadcastedChat();
        void handleGameRules();
        void handleForceLeave();
        void handleAckLeaveLobby();

        typedef size_t (ClientNetwork::*ComponentParser)(const std::vector<uint64_t> &, size_t, ecs::Entity);
        std::map<uint64_t, ComponentParser> _componentParsers;

        size_t parsePlayerTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseTransformComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseSpeedComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseHealthComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseColliderComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseShootingStatsComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseScoreComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseDamageComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseLifetimeComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseVelocityComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseControllableTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseEnemyProjectileTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseGameZoneColliderTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseMobTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseObstacleTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parsePlayerProjectileTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseScoreTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseShooterTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseProjectilePassThroughTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseProjectilePrefabComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseGameZoneComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseAnimationStateComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);
        size_t parseChargedShotComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId);

        DLLoader<createNetworkLib_t> _networloader;
        DLLoader<createBuffer_t> _bufferloader;
        DLLoader<createPacket_t> _packetloader;

        std::shared_ptr<net::INetwork> _network;
        std::shared_ptr<IBuffer> _receptionBuffer;
        std::shared_ptr<IBuffer> _sendBuffer;
        std::shared_ptr<pm::IPacketManager> _packet;

        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<gsm::IGameStateMachine> _gsm;

        uint32_t _sequenceNumber;
        uint16_t _port;
        std::string  _ip;
        std::string _name;
        std::vector<std::string> _clientNames;
        std::vector<std::pair<std::string, std::string>> _lastMessages;
        bool _isDebug;
        bool _expectingLoginResponse = false;
        bool _expectingProfileResponse = false;
        bool _expectingRegisterResponse = false;


        uint8_t _idClient;
        std::shared_ptr<net::INetworkEndpoint> _serverEndpoint;

        std::queue<NetworkEvent> _eventQueue;
        std::mutex _queueMutex;
        std::condition_variable _queueCond;

        std::unordered_map<size_t, ecs::Entity> _serverToLocalEntityMap;

        std::unordered_map<ecs::Entity, std::string> _lastReceivedAnimationState;

        std::string _lobbyCode;
        int _retryCount;
        bool _shouldConnect;

        std::vector<std::pair<std::string, std::string>> _leaderboardData;
        bool _leaderboardDataUpdated = false;
        std::vector<std::string> _profileData;
        bool _profileDataUpdated = false;

        std::chrono::steady_clock::time_point _connectionAttemptTime;
};

#endif /* !CLIENTNETWORK_HPP_ */
