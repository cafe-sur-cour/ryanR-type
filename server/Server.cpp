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

#include "Server.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../common/Error/ServerErrror.hpp"
#include "Signal.hpp"

rserv::Server::Server() : _nextClientId(1), _sequenceNumber(1) {
    this->_clients = {};
    this->_config = nullptr;
    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
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
    std::cout << "[SERVER] Starting server..." << std::endl;
    this->setState(1);

    Signal::setupSignalHandlers();
    while (this->getState() == 1 && !Signal::stopFlag) {
        processIncomingPackets();
        if (std::cin.eof()) {
            std::cout << "EOF received (Ctrl+D pressed)" << std::endl;
            break;
        }
    }
    if (Signal::stopFlag) {
        std::cout << "[SERVER] Received signal, stopping server" << std::endl;
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
        std::cerr << "[SERVER] Info: Server is not running." << std::endl;
        return;
    }
    _network->stop();
    this->setState(0);
    std::cout << "[SERVER] Server stopped." << std::endl;
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
    if (received.second.at(0) == 0x93) {
        this->_packet->unpack(received.second);
        return;
    }

    this->_packet->unpack(received.second);
    if (this->_packet->getType() == 0x01) {
        this->processConnections(received.first);
    } else {
        // Other packet types will be handled here
    }

    this->_packet->reset();
}

bool rserv::Server::processConnections(asio::ip::udp::endpoint id) {
    if (!_network) {
        std::cerr << "[SERVER] Warning: Network not initialized" << std::endl;
        return false;
    }

    if (this->_nextClientId > this->getConfig()->getNbClients()) {
        std::cerr << "[SERVER] Warning: Maximum clients reached" << std::endl;
        return false;
    }

    std::vector<uint8_t> header =
        this->_packet->pack(0, this->_sequenceNumber, 0x02);
    if (!this->_network->sendTo(id, header)) {
        std::cerr << "[SERVER NETWORK] Failed to send connection acceptance header to "
            << id.address().to_string() << ":" << id.port() << std::endl;
        return false;
    }
    std::vector<uint8_t> payload =
        this->_packet->pack({0x02, static_cast<uint64_t>(this->_nextClientId)});
    if (!this->_network->sendTo(id, payload)) {
        std::cerr << "[SERVER NETWORK] Failed to send acceptation payload to "
            << id.address().to_string() << ":" << id.port() << std::endl;
        return false;
    }

    this->_clients.push_back(std::make_tuple(this->_nextClientId, id, ""));
    this->_nextClientId++;
    return true;
}

bool rserv::Server::processDisconnections(uint8_t idClient) {
    (void)idClient;
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
    if (_network) {
        return _network->getActiveConnections();
    }
    return {};
}

size_t rserv::Server::getClientCount() const {
    if (_network) {
        return _network->getConnectionCount();
    }
    return 0;
}

void rserv::Server::onClientConnected(uint8_t idClient) {
    std::cout << "[SERVER] Client " << static_cast<int>(idClient) << " connected" << std::endl;
    // Add game-specific logic here
}

void rserv::Server::onClientDisconnected(uint8_t idClient) {
    std::cout << "[SERVER] Client " << static_cast<int>(idClient)
        << " disconnected" << std::endl;
    // Add game-specific cleanup logic here
}

void rserv::Server::onPacketReceived(
    uint8_t idClient, const pm::IPacketManager &packet) {
    std::cout << "[SERVER] Received packet from client "
        << static_cast<int>(idClient) << std::endl;
    (void)packet;
    // Add game-specific packet processing logic here
}
