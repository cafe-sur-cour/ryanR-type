/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server
*/

#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <utility>
#include <cstring>
#include <queue>

#include "Server.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../common/Error/ServerErrror.hpp"
#include "../common/debug.hpp"
#include "Signal.hpp"

rserv::Server::Server() : _nextClientId(1), _sequenceNumber(1) {
    this->_clients = {};
    this->_config = nullptr;
    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
    this->_eventQueue = std::make_shared<std::queue<std::tuple<uint8_t,
        constants::EventType, double, double>>>();
    this->_config = std::make_shared<rserv::ServerConfig>();
}

rserv::Server::~Server() {
    if (this->getState() == 1)
        this->stop();
    if (this->_network != nullptr &&
        this->_networloader.getHandler() != nullptr) {
        this->_network.reset();
        this->_networloader.Close();
    }
    if (this->_buffer != nullptr &&
        this->_bufferloader.getHandler() != nullptr) {
        this->_buffer.reset();
        this->_bufferloader.Close();
    }
    if (this->_packet != nullptr &&
        this->_packetloader.getHandler() != nullptr) {
        this->_packet.reset();
        this->_packetloader.Close();
    }
}

void rserv::Server::init() {
    if (!this->_config) {
        throw err::ServerError("[SERVER] Server configuration not set",
            err::ServerError::CONFIG_ERROR);
    }

    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
    this->_network->init(this->getPort(), this->_config->getIp());
    this->setState(0);
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Server initialized on port " + std::to_string(this->getPort()),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

void rserv::Server::start() {
    if (this->getState() == 1) {
        std::cerr << "[SERVER] Info: Server is already running" << std::endl;
        return;
    }
    if (this->getState() == -1) {
        throw err::ServerError(
            "[SERVER] Error: init() must be called before start()",
            err::ServerError::INTERNAL_ERROR);
    }
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Starting server...",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    this->setState(1);

    Signal::setupSignalHandlers();
    while (this->getState() == 1 && !Signal::stopFlag) {
        this->processIncomingPackets();
        if (std::cin.eof()) {
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "EOF received (Ctrl+D pressed)",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            break;
        }
    }
    if (Signal::stopFlag) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "Termination signal received",
            debug::debugType::NETWORK, debug::debugLevel::INFO);
        this->stop();
    }
}

void rserv::Server::stop() {
    if (this->getState() == -1) {
        throw err::ServerError(
            "[SERVER] Error: init() must be called before stop()",
            err::ServerError::INTERNAL_ERROR);
    }
    if (this->getState() == 0) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Info: Server is not running.",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return;
    }
    _network->stop();
    this->setState(0);
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "[SERVER] Server stopped.",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
}

rserv::Server::operator int() const noexcept {
    return this->getState();
}

void rserv::Server::setConfig(std::shared_ptr<rserv::ServerConfig> config) {
    this->_config = config;
}

std::shared_ptr<rserv::ServerConfig> rserv::Server::getConfig() const {
    return this->_config;
}

uint16_t rserv::Server::getPort() const {
    return this->_config->getPort();
}

void rserv::Server::setPort(uint16_t port) {
    this->_config->setPort(port);
}

int rserv::Server::getState() const {
    return this->_config->getState();
}

void rserv::Server::setState(int state) {
    this->_config->setState(state);
}

int rserv::Server::getFd() const {
    return this->_config->getFd();
}

void rserv::Server::setFd(int fd) {
    this->_config->setFd(fd);
}

std::shared_ptr<net::INetwork> rserv::Server::getNetwork() const {
    return _network;
}

void rserv::Server::setNetwork(std::shared_ptr<net::INetwork> network) {
    _network = network;
}

void rserv::Server::processIncomingPackets() {
    if (!_network) {
        std::cerr << "[SERVER] Warning: Network not initialized" << std::endl;
        return;
    }

    std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> received = _network->receiveAny();
    if (received.second.empty()) {
        return;
    }

    if (!this->_packet) {
        std::cerr << "[SERVER] Warning: Packet manager not initialized" << std::endl;
        return;
    }

    this->_packet->unpack(received.second);

    if (this->_packet->getType() == 0x01) {
        this->processConnections(received.first);
    } else if (this->_packet->getType() == 0x04) {
        this->processEvents(this->_packet->getIdClient());
    } else {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Packet received of type "
            + std::to_string(static_cast<int>(this->_packet->getType()))
            + " from client: " + std::to_string(this->_packet->getIdClient()),
            debug::debugType::NETWORK, debug::debugLevel::INFO);
    }

    this->_packet->reset();
}

bool rserv::Server::processConnections(asio::ip::udp::endpoint id) {
    if (!_network) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Network not initialized",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    if (this->_nextClientId > this->getConfig()->getNbClients()) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER] Warning: Maximum clients reached",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        return false;
    }

    std::vector<uint8_t> packet = this->_packet->pack(0, this->_sequenceNumber, 0x02
        , std::vector<uint64_t>{static_cast<uint64_t>(this->_nextClientId)});
    if (!this->_network->sendTo(id, packet)) {
        debug::Debug::printDebug(this->_config->getIsDebug(),
            "[SERVER NETWORK] Failed to send connection acceptance header to "
            + id.address().to_string() + ":" + std::to_string(id.port()),
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    this->_clients.push_back(std::make_tuple(this->_nextClientId, id, ""));
    this->_nextClientId++;
    return true;
}

bool rserv::Server::processDisconnections(uint8_t idClient) {
    for (auto &client : this->_clients) {
        if (std::get<0>(client) == idClient) {
            this->_clients.erase(
                std::remove(this->_clients.begin(), this->_clients.end(), client),
                this->_clients.end());
            this->_nextClientId--;
            debug::Debug::printDebug(this->_config->getIsDebug(),
                "Client " + std::to_string(idClient)
                + " disconnected and removed from the lobby",
                debug::debugType::NETWORK, debug::debugLevel::INFO);
            return true;
        }
    }
    return false;
}

bool rserv::Server::processEvents(uint8_t idClient) {
    constants::EventType eventType =
        static_cast<constants::EventType>(this->_packet->getPayload().at(0));

    uint64_t param1Bits = this->_packet->getPayload().at(1);
    uint64_t param2Bits = this->_packet->getPayload().at(2);

    double param1;
    double param2;
    std::memcpy(&param1, &param1Bits, sizeof(double));
    std::memcpy(&param2, &param2Bits, sizeof(double));

    this->_eventQueue->push(std::tuple(idClient, eventType, param1, param2));
    return true;
}

void rserv::Server::broadcastPacket() {
    std::vector<uint8_t> packedData;
    if (_network) {
        _network->broadcast(packedData);
    }
}

void rserv::Server::sendToClient(uint8_t idClient) {
    (void)idClient;
}

std::vector<uint8_t> rserv::Server::getConnectedClients() const {
    return {};
}

size_t rserv::Server::getClientCount() const {
    return this->_clients.size();
}

std::shared_ptr<std::queue<std::tuple<uint8_t,
    constants::EventType, double, double>>> rserv::Server::getEventQueue() {
    return this->_eventQueue;
}

bool rserv::Server::hasEvents() const {
    return !this->_eventQueue->empty();
}

void rserv::Server::onClientConnected(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Client " + std::to_string(idClient) + " connected",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    // Add game-specific logic here
}

void rserv::Server::onClientDisconnected(uint8_t idClient) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Client " + std::to_string(idClient) + " disconnected",
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    // Add game-specific cleanup logic here
}

void rserv::Server::onPacketReceived(
    uint8_t idClient, const pm::IPacketManager &packet) {
    debug::Debug::printDebug(this->_config->getIsDebug(),
        "Packet received from client " + std::to_string(idClient)
        + " of type " + std::to_string(static_cast<int>(packet.getType())),
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    (void)packet;
    // Add game-specific packet processing logic here
}
