/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UnixClientNetwork
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "ClientNetwork.hpp"
#include "../NetworkSocket.hpp"
#include "../NetworkResolver.hpp"
#include "../NetworkErrorCode.hpp"
#include "../AsioEventLoop.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

UnixClientNetwork::UnixClientNetwork() : _connected(false) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;

    this->_serverEndpoint = {};
}

UnixClientNetwork::~UnixClientNetwork() {
    if (_connected) {
        disconnect();
    }
}


void UnixClientNetwork::init(uint16_t port, const std::string host) {
    NetworkErrorCode ec;

    if (!this->_socket) {
        this->_socket = NetworkSocketFactory::createUdpSocket(_eventLoop);
    }

    if (!this->_socket->isOpen()) {
        if (!this->_socket->open(ec)) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to open socket: ") + ec.message());
        }
    }
    try {
        auto resolver = NetworkResolverFactory::createUdpResolver(_eventLoop);
        auto endpoints = resolver->resolve(host, std::to_string(port), ec);
        if (ec || endpoints.empty()) {
            throw std::runtime_error(
                "[CLIENT NETWORK] Failed to resolve host '" + host + "': " + 
                (ec.hasError() ? ec.message() : "no results"));
        }
        this->_serverEndpoint = endpoints[0];
        if (!this->_socket->setNonBlocking(true, ec)) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to set non-blocking mode: ") +
                ec.message());
        }
        this->setConnectionState(ConnectionState::CONNECTING);
    } catch (const std::exception& e) {
        this->_connected = false;
        throw std::runtime_error(
            std::string("[CLIENT NETWORK] Connection failed: ") + e.what());
    }
}

void UnixClientNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            NetworkErrorCode ec;
            _socket->close(ec);
        } catch (const std::exception&) {
        }
    }
    _connected = false;
    _isRunning = false;
}

void UnixClientNetwork::disconnect() {
    if (_socket && _socket->isOpen()) {
        try {
            NetworkErrorCode ec;
            _socket->close(ec);
        } catch (const std::exception&) {
        }
    }
    _connected = false;
    this->setConnectionState(ConnectionState::DISCONNECTED);
    if (this->_onDisconnectCallback) {
        this->_onDisconnectCallback(0);
    }
}

bool UnixClientNetwork::isConnected() const {
    return this->_connected;
}

bool UnixClientNetwork::sendTo(const NetworkEndpoint& endpoint, std::vector<uint8_t> packet) {
    try {
        if (!this->_socket || !this->_socket->isOpen()) {
            std::cerr << "[CLIENT NETWORK] Socket is not open for sending." << std::endl;
            return false;
        }
        if (packet.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to send." << std::endl;
            return false;
        }
        NetworkErrorCode ec;
        _socket->sendTo(packet, endpoint, 0, ec);
        if (ec) {
            std::cerr << "[CLIENT NETWORK] Send error: " << ec.message() << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Send error: " << e.what() << std::endl;
        return false;
    }
}

bool UnixClientNetwork::broadcast(const std::vector<NetworkEndpoint>& endpoints,
    std::vector<uint8_t> data) {
    try {
    for (const auto &endpoint : endpoints) {
        if (!this->sendTo(endpoint, data)) {
            std::cerr << "[CLIENT NETWORK] Broadcast error to endpoint: "
                << endpoint.getAddress() << ":" << endpoint.getPort() << std::endl;
            return false;
        }
        if (data.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to broadcast." << std::endl;
            return false;
        }
        NetworkErrorCode ec;
        _socket->sendTo(data, this->_serverEndpoint, 0, ec);
        if (ec) {
            std::cerr << "[CLIENT NETWORK] Broadcast error: " << ec.message() << std::endl;
            return false;
        }
    }
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Broadcast error: " << e.what() << std::endl;
    }
    return true;
}

bool UnixClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->isOpen()) {
        return false;
    }
    // En mode non-bloquant, on peut tenter une lecture pour vérifier
    // Pour simplifier, on retourne toujours false ici et on gère dans receiveFrom
    return false;
}


std::vector<uint8_t> UnixClientNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;

    NetworkErrorCode ec;
    std::vector<uint8_t> buffer(65536); // Taille maximale UDP
    NetworkEndpoint sender;

    std::size_t bytesReceived = _socket->receiveFrom(buffer, sender, 0, ec);

    if (ec) {
        if (ec == NetworkError::WOULD_BLOCK || ec == NetworkError::AGAIN) {
            return std::vector<uint8_t>();
        }
        std::cerr << "[CLIENT NETWORK] Receive error: " << ec.message() << std::endl;
        return std::vector<uint8_t>();
    }

    if (bytesReceived > 0) {
        buffer.resize(bytesReceived);
        this->_serverEndpoint = sender;
        return buffer;
    }
    return std::vector<uint8_t>();
}

std::pair<NetworkEndpoint, std::vector<uint8_t>> UnixClientNetwork::receiveAny() {
    return {};
}

}  //  namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
