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

#include "Server.hpp"
#include "../libs/Network/Unix/ServerNetwork.hpp"
#include "../common/Error/ServerErrror.hpp"
#include "Signal.hpp"

rserv::Server::Server() {
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
    this->setState(0);
    if (!this->_config) {
        throw err::ServerError("[Server] Server configuration not set",
            err::ServerError::CONFIG_ERROR);
    }

    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
}

void rserv::Server::start() {
    if (this->getState() == 1) {
        std::cerr << "[Server] Error: Server is already running" << std::endl;
        return;
    }
    if (this->getState() == -1) {
        throw err::ServerError("[Server] init() must be called before start()",
            err::ServerError::INTERNAL_ERROR);
    }
    std::cout << "[Server] Starting server..." << std::endl;
    this->setState(1);

    Signal::setupSignalHandlers();

    while (this->getState() == 1 && !Signal::stopFlag) {
        processConnections();
        processIncomingPackets();
        if (std::cin.eof()) {
            std::cout << "EOF received (Ctrl+D pressed)" << std::endl;
            break;
        }
    }
    if (Signal::stopFlag) {
        std::cout << "[Server] Received signal, stopping server" << std::endl;
        this->stop();
    }
}

void rserv::Server::stop() {
    if (this->getState() == -1) {
        throw err::ServerError("[Server] init() must be called before stop()",
            err::ServerError::INTERNAL_ERROR);
    }
    if (this->getState() == 0) {
        std::cerr << "[Server] Error: Server is not running." << std::endl;
        return;
    }
    this->setState(0);
    _network->stop();
    std::cout << "[Server] Server stopped." << std::endl;
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

unsigned int rserv::Server::getPort() const {
    return this->_config->getPort();
}

void rserv::Server::setPort(unsigned int port) {
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

void rserv::Server::processConnections() {
    if (!_network) return;

    int newClientId = _network->acceptConnection();
    while (newClientId != -1) {
        // Connection callback is already handled by network layer
        std::cout << "[Server] New client connected with ID: "
            << newClientId << std::endl;
        newClientId = _network->acceptConnection();
    }
}

void rserv::Server::processIncomingPackets() {
    if (!_network) return;

    while (_network->hasIncomingData()) {
        int senderId = -1;
        (void)senderId;
        // IPacketManager packet = _network->receiveFrom(senderId);

        // if (senderId != -1) {
        //     onPacketReceived(senderId, packet);
        // }
    }
}

void rserv::Server::broadcastPacket() {
    // if (_network) {
    //     _network->broadcast(*this->_packet);
    // }
}

void rserv::Server::sendToClient(int idClient) {
    (void)idClient;
    // if (_network) {
    //     _network->sendTo(idClient, *this->_packet);
    // }
}

std::vector<int> rserv::Server::getConnectedClients() const {
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

void rserv::Server::onClientConnected(int idClient) {
    std::cout << "[Server] Client " << idClient << " connected" << std::endl;
    // Add game-specific logic here
}

void rserv::Server::onClientDisconnected(int idClient) {
    std::cout << "[Server] Client " << idClient << " disconnected" << std::endl;
    // Add game-specific cleanup logic here
}

void rserv::Server::onPacketReceived(
    int idClient, const pm::IPacketManager &packet) {
    std::cout << "[Server] Received packet from client "
        << idClient << std::endl;
    (void)packet;
    // Add game-specific packet processing logic here
}
