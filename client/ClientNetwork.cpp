/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#include "ClientNetwork.hpp"

ClientNetwork::ClientNetwork() {
    this->_port = 0;
    this->_ip = 0;

    this->_network = nullptr;
    this->_buffer = nullptr;
    this->_packet = nullptr;
}

ClientNetwork::~ClientNetwork() {
}

void ClientNetwork::init() {
    if (this->_port == 0 || this->_ip == 0) {
        throw std::runtime_error("[ClientNetwork] Port or IP not set");
    }
    this->loadNetworkLibrary();
    this->loadBufferLibrary();
    this->loadPacketLibrary();
}

void ClientNetwork::start() {

}

void ClientNetwork::stop() {

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

void ClientNetwork::sendData(const IPacket &data, size_t size) {
    (void)data;
    (void)size;
}

IPacket &ClientNetwork::receiveData(const IBuffer &buffer, size_t size) const {
    (void)buffer;
    (void)size;
    return *this->_packet; /* Temporary dereferensment*/
}


void ClientNetwork::loadNetworkLibrary() {
    if (!_networloader.Open(pathLoad "/" networkLib)) {
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
    if (!_bufferloader.Open(pathLoad "/" bufferLib)) {
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
    if (!_packetloader.Open(pathLoad "/" packetLib)) {
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
    _packet = std::shared_ptr<IPacket>
        (reinterpret_cast<IPacket *>(createPacket()));
    if (!_packet) {
        throw std::runtime_error("[ClientNetwork] Loading packet lib failed");
    }
}
