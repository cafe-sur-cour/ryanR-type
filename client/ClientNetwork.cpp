/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/
#include <cstring>

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <memory>

#include "ClientNetwork.hpp"
#include "../libs/Network/common.hpp"
#include "constants.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/translationToECS.hpp"
#include "../common/Signal/Signal.hpp"
#include "../common/debug.hpp"
#include "../common/constants.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";
    this->_name = "";
    this->_idClient = 0;
    this->_eventQueue = std::queue<NetworkEvent>();

    this->_isConnected = false;
    this->_network = nullptr;
    this->_receptionBuffer = nullptr;
    this->_sendBuffer = nullptr;
    this->_packet = nullptr;
    this->_sequenceNumber = 0;
    this->_isConnected = false;
    this->_ready = false;
    this->_isDebug = false;
    this->_resourceManager = nullptr;
    this->_gsm = nullptr;
    this->_clientNames = {};
    this->_serverEndpoint = {};
    this->_lobbyCode = "";
    this->_isConnectedToLobby = false;
    this->_isLobbyMaster = false;
    this->_shouldDisconnect = false;

    this->_shouldConnect = false;
    this->_connectionAttemptTime = std::chrono::steady_clock::now();
    this->_lastLeaveLobbyTime = std::chrono::steady_clock::now() - std::chrono::seconds(10);

    this->_connectedClients = 0;
    this->_readyClients = 0;
    this->_clientId = 0;
    this->_retryCount = 0;
    this->_clientReadyStatus = false;

    // Initialize packet handlers
    _packetHandlers[constants::PACKET_NO_OP] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_CONNECTION] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_ACCEPT] = &ClientNetwork::handleConnectionAcceptation;
    _packetHandlers[constants::PACKET_DISC] = &ClientNetwork::disconnectionPacket;
    _packetHandlers[constants::PACKET_EVENT] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_GAME_STATE_BATCH] =
        &ClientNetwork::handleBatchedGameState;
    _packetHandlers[constants::PACKET_END_GAME] = &ClientNetwork::handleEndGame;
    _packetHandlers[constants::PACKET_CAN_START] = &ClientNetwork::handleCanStart;
    _packetHandlers[constants::PACKET_SPAWN] = &ClientNetwork::handleEntitySpawn;
    _packetHandlers[constants::PACKET_DEATH] = &ClientNetwork::handleEntityDeath;
    _packetHandlers[constants::PACKET_WHOAMI] = &ClientNetwork::handleWhoAmI;
    _packetHandlers[constants::PACKET_SERVER_STATUS] = &ClientNetwork::handleServerStatus;
    _packetHandlers[constants::PACKET_SEND_LOBBY_CODE] = &ClientNetwork::handleCode;
    _packetHandlers[constants::PACKET_LEVEL_COMPLETE] = &ClientNetwork::handleLevelComplete;
    _packetHandlers[constants::PACKET_NEXT_LEVEL] = &ClientNetwork::handleNextLevel;
    _packetHandlers[constants::PACKET_LOBBY_CONNECT_VALUE] =
        &ClientNetwork::handleLobbyConnectValue;
    _packetHandlers[constants::PACKET_CONNECT_USER] = &ClientNetwork::handleConnectUser;
    _packetHandlers[constants::PACKET_LEADERBOARD] = &ClientNetwork::handleLeaderboard;
    _packetHandlers[constants::PACKET_REGISTER_FAIL] = &ClientNetwork::handleRegisterFail;
    _packetHandlers[constants::PACKET_PROFILE] = &ClientNetwork::handleProfile;
    _packetHandlers[constants::PACKET_BROADCASTED_CHAT] =
        &ClientNetwork::handleBroadcastedChat;
    _packetHandlers[constants::PACKET_GAME_RULES] = &ClientNetwork::handleGameRules;
    _packetHandlers[constants::PACKET_FORCE_LEAVE] = &ClientNetwork::handleForceLeave;
    _packetHandlers[constants::PACKET_ACK_LEAVE_LOBBY] = &ClientNetwork::handleAckLeaveLobby;

    _componentParsers[PLAYER_TAG] = &ClientNetwork::parsePlayerTagComponent;
    _componentParsers[TRANSFORM] = &ClientNetwork::parseTransformComponent;
    _componentParsers[SPEED_COMP] = &ClientNetwork::parseSpeedComponent;
    _componentParsers[HEALTH] = &ClientNetwork::parseHealthComponent;
    _componentParsers[COLLIDER] = &ClientNetwork::parseColliderComponent;
    _componentParsers[SHOOTING_STATS] = &ClientNetwork::parseShootingStatsComponent;
    _componentParsers[SCORE] = &ClientNetwork::parseScoreComponent;
    _componentParsers[DAMAGE] = &ClientNetwork::parseDamageComponent;
    _componentParsers[LIFETIME] = &ClientNetwork::parseLifetimeComponent;
    _componentParsers[VELOCITY] = &ClientNetwork::parseVelocityComponent;
    _componentParsers[PROJECTILE_PREFAB] = &ClientNetwork::parseProjectilePrefabComponent;
    _componentParsers[OBSTACLE_TAG] = &ClientNetwork::parseObstacleTagComponent;
    _componentParsers[ANIMATION_STATE] = &ClientNetwork::parseAnimationStateComponent;
    _componentParsers[CHARGED_SHOT_COMP] = &ClientNetwork::parseChargedShotComponent;
}

ClientNetwork::~ClientNetwork() {
    this->stop();
    this->_gsm.reset();
    this->_network.reset();
    if (this->_packet != nullptr) {
            this->_packet->clearAllHandlers();
        this->_packet.reset();
    }
    this->_receptionBuffer.reset();
    this->_sendBuffer.reset();
}

void ClientNetwork::setResourceManager(std::shared_ptr<ResourceManager> resourceManager) {
    this->_resourceManager = resourceManager;
}

void ClientNetwork::setGameStateMachine(std::shared_ptr<gsm::IGameStateMachine> gsm) {
    this->_gsm = gsm;
}

std::shared_ptr<gsm::IGameStateMachine> ClientNetwork::getGameStateMachine() const {
    return this->_gsm;
}

void ClientNetwork::init() {
    if (this->_port == 0 || this->_ip == "") {
        throw err::ClientNetworkError(
            "[ClientNetwork] Port and IP must be set before init()",
            err::ClientNetworkError::CONFIG_ERROR);
    }
    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
    this->_network->init(
        this->_port,
        this->_ip
    );
    this->_serverEndpoint = std::make_shared<NetworkEndpoint>(this->_ip,
        static_cast<uint16_t>(this->_port));
}

void ClientNetwork::connect() {
    this->redoServerEndpoint();
    this->_connectionAttemptTime = std::chrono::steady_clock::now();
    this->_shouldConnect = true;
    if (this->_network->getConnectionState() == net::ConnectionState::DISCONNECTED) {
        this->_idClient = 0;
        this->_sequenceNumber = 0;
        debug::Debug::printDebug(this->_isDebug,
            "Attempting to connect to server...",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
    }
}

void ClientNetwork::stop() {
    if (this->_network == nullptr) {
        return;
    }
    this->_network->stop();
    this->_network->setConnectionState(net::ConnectionState::DISCONNECTED);
    this->_isConnected = false;
    this->_ready = false;
    this->_shouldConnect = false;
    this->_idClient = 0;
    this->_sequenceNumber = 0;
}

uint16_t ClientNetwork::getPort() const {
    return _port;
}

void ClientNetwork::setPort(int port) {
    _port = static_cast<uint16_t>(port);
}

std::string ClientNetwork::getIp() const {
    return _ip;
}

void ClientNetwork::setIp(const std::string &ip) {
    _ip = ip;
}

std::string ClientNetwork::getLobbyCode() const {
    return this->_lobbyCode;
}

void ClientNetwork::setLobbyCode(std::string lobbyCode) {
    this->_lobbyCode = lobbyCode;
}

void ClientNetwork::redoServerEndpoint() {
    this->_serverEndpoint.reset();
    this->_serverEndpoint = std::make_shared<NetworkEndpoint>(this->_ip,
        static_cast<uint16_t>(this->_port));
}

void ClientNetwork::setDebugMode(bool isDebug) {
    this->_isDebug = isDebug;
}

bool ClientNetwork::isDebugMode() const {
    return this->_isDebug;
}

std::shared_ptr<net::INetwork> ClientNetwork::getNetwork() const {
    return this->_network;
}

void ClientNetwork::sendConnectionData(std::vector<uint8_t> packet) {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    this->_network->sendTo(*this->_serverEndpoint, packet);
}

std::string ClientNetwork::getName() const {
    return this->_name;
}

const std::vector<std::pair<std::string, std::string>>&
    ClientNetwork::getLastMessages() const {
    return this->_lastMessages;
}

void ClientNetwork::setName(const std::string &name) {
    this->_name = name;
}

uint8_t ClientNetwork::getIdClient() const {
    return this->_idClient;
}

void ClientNetwork::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}

net::ConnectionState ClientNetwork::getConnectionState() const {
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Network not initialized",
            err::ClientNetworkError::INTERNAL_ERROR);
    }
    return this->_network->getConnectionState();
}

bool ClientNetwork::isConnected() const {
    return this->_isConnected.load();
}

bool ClientNetwork::isReady() const {
    return this->_ready.load();
}

bool ClientNetwork::isConnectedToLobby() const {
    return this->_isConnectedToLobby.load();
}

bool ClientNetwork::isLobbyMaster() const {
    return this->_isLobbyMaster.load();
}

void ClientNetwork::handlePacketType(uint8_t type) {
    if (type < constants::MAX_INDEX_PACKET_TYPE && this->_packetHandlers[type]) {
        (this->*_packetHandlers[type])();
    } else {
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Unknown packet type: " + std::to_string(static_cast<int>(type)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
}

std::pair<int, std::chrono::steady_clock::time_point> ClientNetwork::tryConnection(
    int maxRetries, std::chrono::steady_clock::time_point lastRetryTime) {
    auto currentTime = std::chrono::steady_clock::now();

    if (this->_shouldConnect &&
        this->_network->getConnectionState() == net::ConnectionState::CONNECTING &&
        this->_retryCount < maxRetries &&
        std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastRetryTime).count()
            >= constants::NETWORK_TIMEOUT) {
        debug::Debug::printDebug(this->_isDebug,
            "Retrying connection (attempt " + std::to_string(this->_retryCount + 1) + "/" +
            std::to_string(maxRetries) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
        this->_retryCount++;
        lastRetryTime = currentTime;
    }
    return {this->_retryCount, lastRetryTime};
}

void ClientNetwork::start() {
    Signal::setupSignalHandlers();

    const int maxRetries = constants::MAX_RETRY_CONNECTIONS;
    auto lastRetryTime = std::chrono::steady_clock::now();

    while (!Signal::stopFlag) {
        std::tie(this->_retryCount, lastRetryTime) = tryConnection(maxRetries,
            lastRetryTime);

        if (this->_shouldConnect && !this->_isConnected.load() &&
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - this->_connectionAttemptTime).count() >
                constants::CONNECTION_ATTEMPT_TIMEOUT) {
            this->_shouldConnect = false;
            debug::Debug::printDebug(this->_isDebug,
                "Connection attempt timed out after " +
                std::to_string(constants::CONNECTION_ATTEMPT_TIMEOUT) + " seconds",
                debug::debugType::NETWORK,
                debug::debugLevel::ERROR);
            auto mainMenuState =
                std::make_shared<gsm::MainMenuState>(this->_gsm, this->_resourceManager);
        }

        std::vector<uint8_t> receivedData = this->_network->receiveFrom(this->_idClient);
        if (receivedData.size() > 0) {
            if (this->_packet->unpack(receivedData)) {
                handlePacketType(this->_packet->getType());
            } else {
                debug::Debug::printDebug(this->_isDebug,
                    "[CLIENT] Failed to unpack received packet",
                    debug::debugType::NETWORK,
                    debug::debugLevel::ERROR);
            }
            receivedData.clear();
        }
        if (!this->_eventQueue.empty()) {
            std::lock_guard<std::mutex> lock(this->_queueMutex);
            this->eventPacket(
                this->_eventQueue.front().eventType,
                this->_eventQueue.front().depth
            );
            this->_eventQueue.pop();
        }
    }
    if (this->_network->getConnectionState() != net::ConnectionState::CONNECTED
        && this->_retryCount >= maxRetries) {
        debug::Debug::printDebug(this->_isDebug,
            "Connection failed after " + std::to_string(maxRetries) +
            " attempts. Setting to ERROR_STATE.",
            debug::debugType::NETWORK,
            debug::debugLevel::ERROR);
        this->_network->setConnectionState(net::ConnectionState::ERROR_STATE);
    }
    if (Signal::stopFlag) {
        this->disconnectionPacket();
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Signal received, stopping client network",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->stop();
    }
}

size_t ClientNetwork::getConnectedClients() const {
    return this->_connectedClients.load();
}

size_t ClientNetwork::getReadyClients() const {
    return this->_readyClients.load();
}

uint8_t ClientNetwork::getClientId() const {
    return this->_clientId.load();
}

bool ClientNetwork::getClientReadyStatus() const {
    return this->_clientReadyStatus.load();
}

void ClientNetwork::addToEventQueue(const NetworkEvent &event) {
    std::lock_guard<std::mutex> lock(this->_queueMutex);
    this->_eventQueue.push(event);
    this->_queueCond.notify_one();
}

const std::vector<std::pair<std::string, std::string>>&
    ClientNetwork::getLeaderboardData() const {
    return _leaderboardData;
}

bool ClientNetwork::isLeaderboardDataUpdated() const {
    return _leaderboardDataUpdated;
}

void ClientNetwork::clearLeaderboardDataUpdateFlag() {
    _leaderboardDataUpdated = false;
}

const std::vector<std::string>& ClientNetwork::getProfileData() const {
    return _profileData;
}

bool ClientNetwork::isProfileDataUpdated() const {
    return _profileDataUpdated;
}

void ClientNetwork::clearProfileDataUpdateFlag() {
    _profileDataUpdated = false;
}

void ClientNetwork::clearEntitiesAndMappings() {
    if (_resourceManager) {
        auto registry = _resourceManager->get<ecs::Registry>();
        if (registry) {
            registry->clearAllEntities();
            debug::Debug::printDebug(this->_isDebug,
                "[CLIENT] Cleared all entities from client registry",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    }

    _serverToLocalEntityMap.clear();
    _lastReceivedAnimationState.clear();

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Cleared all entity mappings",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
}
