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
#include <queue>
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/Signal/Signal.hpp"
#include "../common/debug.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";
    this->_name = "Doof";
    this->_idClient = 0;
    this->_eventQueue = std::queue<NetworkEvent>();

    this->_network = nullptr;
    this->_receptionBuffer = nullptr;
    this->_sendBuffer = nullptr;
    this->_packet = nullptr;
    this->_sequenceNumber = 0;
    this->_isConnected = false;
    this->_isDebug = false;

    // Initialize packet handlers
    _packetHandlers[0x00] = &ClientNetwork::handleNoOp;
    _packetHandlers[0x01] = &ClientNetwork::handleNoOp;  // Client sends this
    _packetHandlers[0x02] = &ClientNetwork::handleConnectionAcceptation;
    _packetHandlers[0x03] = &ClientNetwork::handleNoOp;  // Client sends this
    _packetHandlers[0x04] = &ClientNetwork::handleNoOp;  // Client sends this
    _packetHandlers[0x05] = &ClientNetwork::handleGameState;
    _packetHandlers[0x06] = &ClientNetwork::handleMapSend;
    _packetHandlers[0x07] = &ClientNetwork::handleEndMap;
    _packetHandlers[0x08] = &ClientNetwork::handleEndGame;
    _packetHandlers[0x09] = &ClientNetwork::handleCanStart;
}

ClientNetwork::~ClientNetwork() {
    this->stop();
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

void ClientNetwork::stop() {
    if (this->_network != nullptr
        && this->_networloader.getHandler() != nullptr) {
        this->_network.reset();
        this->_networloader.Close();
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
    if (this->_packet != nullptr
        && this->_packetloader.getHandler() != nullptr) {
        this->_packet.reset();
        this->_packetloader.Close();
    }
}

void ClientNetwork::handlePacketType(uint8_t type) {
    if (this->_packet->getPayload().empty()) {
        return;
    }
    if (type < 10) {
        (this->*_packetHandlers[type])();
    } else {
        debug::Debug::printDebug(this->_isDebug,
            "[Client] Unknown packet type: " + std::to_string(static_cast<int>(type)),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
}


void ClientNetwork::tryConnection(const int maxRetries, int &retryCount,
    std::chrono::steady_clock::time_point &lastRetryTime) {
    auto currentTime = std::chrono::steady_clock::now();

    if (this->_network->getConnectionState() == net::ConnectionState::CONNECTING &&
        retryCount < maxRetries &&
        std::chrono::duration_cast<std::chrono::seconds>(currentTime -
            lastRetryTime).count() >= 2) {
        debug::Debug::printDebug(this->_isDebug,
            "Retrying connection (attempt " + std::to_string(retryCount + 1) + "/"
            + std::to_string(maxRetries) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
        retryCount++;
        lastRetryTime = currentTime;
    }
}

void ClientNetwork::start() {
    Signal::setupSignalHandlers();

    const int maxRetries = 3;
    int retryCount = 0;
    auto lastRetryTime = std::chrono::steady_clock::now();

    if (this->_network->getConnectionState() == net::ConnectionState::CONNECTING) {
        debug::Debug::printDebug(this->_isDebug,
            "Attempting initial connection (attempt 1/" + std::to_string(maxRetries) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->connectionPacket();
        retryCount++;
        lastRetryTime = std::chrono::steady_clock::now();
    }

    while (!Signal::stopFlag) {
        tryConnection(maxRetries, retryCount, lastRetryTime);
        std::vector<uint8_t> receivedData = this->_network->receiveFrom(this->_idClient);
        if (receivedData.size() > 0) {
            this->_packet->unpack(receivedData);
            handlePacketType(this->_packet->getType());
            receivedData.clear();
        }
        if (!this->_eventQueue.empty()) {
            std::lock_guard<std::mutex> lock(this->_queueMutex);
            this->eventPacket(
                this->_eventQueue.front().eventType,
                this->_eventQueue.front().depth,
                this->_eventQueue.front().direction
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
            "[Client] Signal received, stopping client network",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
        this->stop();
    }
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

void ClientNetwork::addToEventQueue(const NetworkEvent &event) {
    std::lock_guard<std::mutex> lock(this->_queueMutex);
    this->_eventQueue.push(event);
    this->_queueCond.notify_one();
}

bool ClientNetwork::getEventFromQueue(NetworkEvent &event) {
    std::unique_lock<std::mutex> lock(this->_queueMutex);
    if (this->_eventQueue.empty()) {
        return false;
    }
    event = this->_eventQueue.front();
    this->_eventQueue.pop();
    return true;
}

