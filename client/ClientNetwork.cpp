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
#include "constants.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/translationToECS.hpp"
#include "../common/Signal/Signal.hpp"
#include "../common/debug.hpp"
#include "../common/constants.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";
    this->_name = "Doof";
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

    this->_shouldConnect = false;

    this->_connectedClients = 0;
    this->_readyClients = 0;
    this->_clientId = 0;
    this->_clientReadyStatus = false;

    // Initialize packet handlers
    _packetHandlers[constants::PACKET_NO_OP] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_CONNECTION] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_ACCEPT] = &ClientNetwork::handleConnectionAcceptation;
    _packetHandlers[constants::PACKET_DISC] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_EVENT] = &ClientNetwork::handleNoOp;
    _packetHandlers[constants::PACKET_GAME_STATE] = &ClientNetwork::handleGameState;
    _packetHandlers[constants::PACKET_END_GAME] = &ClientNetwork::handleEndGame;
    _packetHandlers[constants::PACKET_CAN_START] = &ClientNetwork::handleCanStart;
    _packetHandlers[constants::PACKET_SPAWN] = &ClientNetwork::handleEntitySpawn;
    _packetHandlers[constants::PACKET_DEATH] = &ClientNetwork::handleEntityDeath;
    _packetHandlers[constants::PACKET_WHOAMI] = &ClientNetwork::handleWhoAmI;
    _packetHandlers[SERVER_STATUS_PACKET] = &ClientNetwork::handleServerStatus;

    _componentParsers[PLAYER_TAG] = &ClientNetwork::parsePlayerTagComponent;
    _componentParsers[TRANSFORM] = &ClientNetwork::parseTransformComponent;
    _componentParsers[SPEED_COMP] = &ClientNetwork::parseSpeedComponent;
    _componentParsers[HEALTH] = &ClientNetwork::parseHealthComponent;
    _componentParsers[COLLIDER] = &ClientNetwork::parseColliderComponent;
    _componentParsers[SHOOTING_STATS] = &ClientNetwork::parseShootingStatsComponent;
    _componentParsers[SCORE] = &ClientNetwork::parseScoreComponent;
    _componentParsers[AI_MOVEMENT_PATTERN] = &ClientNetwork::parseAIMovementPatternComponent;
    _componentParsers[DAMAGE] = &ClientNetwork::parseDamageComponent;
    _componentParsers[LIFETIME] = &ClientNetwork::parseLifetimeComponent;
    _componentParsers[VELOCITY] = &ClientNetwork::parseVelocityComponent;
    _componentParsers[PROJECTILE_PREFAB] = &ClientNetwork::parseProjectilePrefabComponent;
    _componentParsers[OBSTACLE_TAG] = &ClientNetwork::parseObstacleTagComponent;
}

ClientNetwork::~ClientNetwork() {
    this->stop();
    if (this->_gsm != nullptr) {
        this->_gsm.reset();
    }

    this->_resourceManager.reset();
    if (this->_packet != nullptr) {
        this->_packet->clearAllHandlers();
        this->_packet.reset();
    }

    if (this->_network != nullptr) {
        this->_network.reset();
    }

    if (this->_receptionBuffer != nullptr) {
        this->_receptionBuffer.reset();
    }
    if (this->_sendBuffer != nullptr) {
        this->_sendBuffer.reset();
    }

    if (this->_bufferloader.getHandler() != nullptr) {
        this->_bufferloader.Close();
    }

    if (this->_packetloader.getHandler() != nullptr) {
        this->_packetloader.Close();
    }

    if (this->_networloader.getHandler() != nullptr) {
        this->_networloader.Close();
    }
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
    this->_serverEndpoint = asio::ip::udp::endpoint(
        asio::ip::address::from_string(this->_ip),
        static_cast<uint16_t>(this->_port)
    );
}

void ClientNetwork::connect() {
    this->_shouldConnect = true;
    if (this->_network->getConnectionState() == net::ConnectionState::DISCONNECTED) {
        debug::Debug::printDebug(this->_isDebug,
            "Attempting to connect to server...",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
    }
}

void ClientNetwork::stop() {
    this->_network->stop();
    this->_network->setConnectionState(net::ConnectionState::DISCONNECTED);
    this->_isConnected = false;
    this->_ready = false;
    this->_shouldConnect = false;
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

void ClientNetwork::redoServerEndpoint() {
    this->_serverEndpoint = asio::ip::udp::endpoint(
        asio::ip::address::from_string(this->_ip),
        static_cast<uint16_t>(this->_port)
    );
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
    this->_network->sendTo(this->_serverEndpoint, packet);
}

std::string ClientNetwork::getName() const {
    return this->_name;
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

void ClientNetwork::handlePacketType(uint8_t type) {
    if (this->_packet->getPayload().empty()) {
        return;
    }
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
    int maxRetries, int retryCount, std::chrono::steady_clock::time_point lastRetryTime) {
    auto currentTime = std::chrono::steady_clock::now();

    if (this->_shouldConnect &&
        this->_network->getConnectionState() == net::ConnectionState::CONNECTING &&
        retryCount < maxRetries &&
        std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastRetryTime).count()
            >= constants::NETWORK_TIMEOUT) {
        debug::Debug::printDebug(this->_isDebug,
            "Retrying connection (attempt " + std::to_string(retryCount + 1) + "/" +
            std::to_string(maxRetries) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
        retryCount++;
        lastRetryTime = currentTime;
    }
    return {retryCount, lastRetryTime};
}

void ClientNetwork::start() {
    Signal::setupSignalHandlers();

    const int maxRetries = constants::MAX_RETRY_CONNECTIONS;
    int retryCount = 0;
    auto lastRetryTime = std::chrono::steady_clock::now();

    while (!Signal::stopFlag) {
        std::tie(retryCount, lastRetryTime) = tryConnection(maxRetries, retryCount,
            lastRetryTime);
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
        && retryCount >= maxRetries) {
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
