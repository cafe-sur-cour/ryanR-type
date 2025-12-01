/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <memory>

#include "ClientNetwork.hpp"
#include "../common/Error/ClientNetworkError.hpp"
#include "../common/Signal/Signal.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = "";

    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
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

uint32_t ClientNetwork::getPort() const {
    return _port;
}

void ClientNetwork::setPort(int port) {
    _port = static_cast<uint32_t>(port);
}

std::string ClientNetwork::getIp() const {
    return _ip;
}

void ClientNetwork::setIp(const std::string &ip) {
    _ip = ip;
}


void ClientNetwork::loadNetworkLibrary() {
    if (!_networloader.Open(pathLoad "/" networkClientLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_networloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createNetworkLib_t createNetwork = _networloader.getSymbol
        ("createNetworkInstance");
    if (!createNetwork) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    _network = std::shared_ptr<net::INetwork>
        (reinterpret_cast<net::INetwork *>(createNetwork()));
    if (!_network) {
        throw err::ClientNetworkError("[ClientNetwork] Loading network lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}

void ClientNetwork::loadBufferLibrary() {
    if (!_bufferloader.Open(pathLoad "/" bufferLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_bufferloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createBuffer_t createBuffer = _bufferloader.getSymbol
        ("createBufferInstance");
    if (!createBuffer) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    _buffer = std::shared_ptr<IBuffer>
        (reinterpret_cast<IBuffer *>(createBuffer()));
    if (!_buffer) {
        throw err::ClientNetworkError("[ClientNetwork] Loading buffer lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}

void ClientNetwork::loadPacketLibrary() {
    if (!_packetloader.Open(pathLoad "/" packetLib sharedLibExt)) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    if (!_packetloader.getHandler()) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    createPacket_t createPacket = _packetloader.getSymbol
        ("createPacketInstance");
    if (!createPacket) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
    _packet = std::shared_ptr<pm::IPacketManager>
        (reinterpret_cast<pm::IPacketManager *>(createPacket()));
    if (!_packet) {
        throw err::ClientNetworkError("[ClientNetwork] Loading packet lib failed",
            err::ClientNetworkError::LIBRARY_LOAD_FAILED);
    }
}
