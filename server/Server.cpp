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
#include "Server.hpp"
#include "../libs/Network/UnixNetwork/UnixNetwork.hpp"

rserv::Server::Server() {
    this->_config = nullptr;
    this->_network = std::make_shared<net::UnixNetwork>();
    
    // Set up network callbacks
    _network->setConnectionCallback([this](int clientId) {
        this->onClientConnected(clientId);
    });
    
    _network->setDisconnectionCallback([this](int clientId) {
        this->onClientDisconnected(clientId);
    });
}

rserv::Server::~Server() {
    if (this->getState() == 1)
        this->stop();
}

void rserv::Server::init() {
    this->setState(0);
    std::cout << "[Server] Initializing server on port " << _config->getPort() << std::endl;
    
    // Initialize network layer
    _network->init(_config->getPort());
    
    std::cout << "[Server] Initialization complete" << std::endl;
}

void rserv::Server::start() {
    if (this->getState() == 1) {
        std::cerr << "[Server] Error: Server is already running" << std::endl;
        return;
    }
    
    if (this->getState() == -1) {
        std::cerr << "[Server] Error: init() must be called before start()" << std::endl;
        return;
    }

    std::cout << "[Server] Starting server..." << std::endl;
    this->setState(1);
    
    // Main server loop
    while (this->getState() == 1) {
        // Process new connections
        processConnections();
        
        // Process incoming packets
        processIncomingPackets();
        
        // Small sleep to prevent busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void rserv::Server::stop() {
    if (this->getState() == -1) {
        std::cerr << "[Server] Error: init() must be called before stop()" << std::endl;
        return;
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
        newClientId = _network->acceptConnection();
    }
}

void rserv::Server::processIncomingPackets() {
    if (!_network) return;
    
    while (_network->hasIncomingData()) {
        int senderId = -1;
        (void)senderId;
        // IPacket packet = _network->receiveFrom(senderId);
        
        // if (senderId != -1) {
        //     onPacketReceived(senderId, packet);
        // }
    }
}

void rserv::Server::broadcastPacket(const IPacket &packet) {
    if (_network) {
        _network->broadcast(packet);
    }
}

void rserv::Server::sendToClient(int idClient, const IPacket &packet) {
    if (_network) {
        _network->sendTo(idClient, packet);
    }
}

std::vector<int> rserv::Server::getConnectedClients() const {
    if (_network) {
        return _network->getActiveConnections();
    }
    return {};
}

int rserv::Server::getClientCount() const {
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

void rserv::Server::onPacketReceived(int idClient, const IPacket &packet) {
    std::cout << "[Server] Received packet from client " << idClient << std::endl;
    (void)packet;
    // Add game-specific packet processing logic here
}
