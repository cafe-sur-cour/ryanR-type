/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#include <thread>
#include <chrono>
#include <iostream>
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Signal/Signal.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = 0;

    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
}

ClientNetwork::~ClientNetwork() {
    this->stop();
}

void ClientNetwork::init() {
    if (this->_port == 0 || this->_ip == 0) {
        throw std::runtime_error("[ClientNetwork] Port or IP not set");
    }
    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
    this->_network->init(this->_port);
}

/* Call this in a separate thread */
void ClientNetwork::start() {
    Signal::setupSignalHandlers();

    while (!Signal::stopFlag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if (Signal::stopFlag) {
        std::cout << "[Client] Received signal, stopping client" << std::endl;
        this->stop();
    }
}

void ClientNetwork::stop() {
    if (this->_network != nullptr
        && this->_networloader.getHandler() != nullptr) {
        this->_network.reset();
        this->_networloader.Close();
    }
    if (this->_buffer != nullptr
        && this->_bufferloader.getHandler() != nullptr) {
            this->_buffer.reset();
            this->_bufferloader.Close();
    }
    if (this->_packet != nullptr
        && this->_packetloader.getHandler() != nullptr) {
        this->_packet.reset();
        this->_packetloader.Close();
    }
}

int ClientNetwork::getPort() const {
    return _port;
}

void ClientNetwork::setPort(int port) {
    _port = port;
}

uint32_t ClientNetwork::getIp() const {
    return _ip;
}

void ClientNetwork::setIp(uint32_t ip) {
    _ip = ip;
}


void ClientNetwork::loadNetworkLibrary() {
    if (!_networloader.Open(pathLoad "/" networkClientLib sharedLibExt)) {
        throw std::runtime_error("[ClientNetwork] Loading network lib failed");
    }
    if (!_networloader.getHandler()) {
        throw std::runtime_error("[ClientNetwork] Loading network lib failed");
    }
    createNetworkLib_t createNetwork = _networloader.getSymbol
        ("createNetworkInstance");
    if (!createNetwork) {
        throw std::runtime_error("[ClientNetwork] Loading network lib failed");
    }
    _network = std::shared_ptr<net::INetwork>
        (reinterpret_cast<net::INetwork *>(createNetwork()));
    if (!_network) {
        throw std::runtime_error("[ClientNetwork] Loading network lib failed");
    }
}

void ClientNetwork::loadBufferLibrary() {
    if (!_bufferloader.Open(pathLoad "/" bufferLib sharedLibExt)) {
        throw std::runtime_error("[ClientNetwork] Loading buffer lib failed");
    }
    if (!_bufferloader.getHandler()) {
        throw std::runtime_error("[ClientNetwork] Loading buffer lib failed");
    }
    createBuffer_t createBuffer = _bufferloader.getSymbol
        ("createBufferInstance");
    if (!createBuffer) {
        throw std::runtime_error("[ClientNetwork] Loading buffer lib failed");
    }
    _buffer = std::shared_ptr<IBuffer>
        (reinterpret_cast<IBuffer *>(createBuffer()));
    if (!_buffer) {
        throw std::runtime_error("[ClientNetwork] Loading buffer lib failed");
    }
}

void ClientNetwork::loadPacketLibrary() {
    if (!_packetloader.Open(pathLoad "/" packetLib sharedLibExt)) {
        throw std::runtime_error("[ClientNetwork] Loading packet lib failed");
    }
    if (!_packetloader.getHandler()) {
        throw std::runtime_error("[ClientNetwork] Loading packet lib failed");
    }
    createPacket_t createPacket = _packetloader.getSymbol
        ("createPacketInstance");
    if (!createPacket) {
        throw std::runtime_error("[ClientNetwork] Loading packet lib failed");
    }
    _packet = std::shared_ptr<pm::IPacketManager>
        (reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!_packet) {
        throw std::runtime_error("[ClientNetwork] Loading packet lib failed");
    }
}
