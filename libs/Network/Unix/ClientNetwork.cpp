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
#include "../common.hpp"
#include "../../../common/interfaces/INetworkSocket.hpp"
#include "../../../common/interfaces/INetworkResolver.hpp"
#include "../../../common/interfaces/INetworkErrorCode.hpp"
#include "../../../common/interfaces/IEventLoop.hpp"
#include "../../../common/interfaces/INetworkEndpoint.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

net::UnixClientNetwork::UnixClientNetwork() : _connected(false) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _serverEndpoint = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;
}

net::UnixClientNetwork::~UnixClientNetwork() {
    if (_connected) {
        disconnect();
    }
}


void net::UnixClientNetwork::init(uint16_t port, const std::string host) {
    auto ec = std::make_shared<NetworkErrorCode>();

    if (!this->_socket) {
        this->_socket = std::make_shared<NetworkSocket>(_eventLoop);
    }

    if (!this->_socket->isOpen()) {
        if (!this->_socket->open(ec)) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to open socket: ") + ec->message());
        }
    }
    try {
        auto resolver = NetworkResolverFactory::createUdpResolver(_eventLoop);
        auto endpoints = resolver->resolve(host, std::to_string(port), ec);
        if (*ec || endpoints.empty()) {
            throw std::runtime_error(
                "[CLIENT NETWORK] Failed to resolve host '" + host + "': " +
                (ec->hasError() ? ec->message() : "no results"));
        }
        this->_serverEndpoint = endpoints.front();
        if (!this->_socket->setNonBlocking(true, ec)) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to set non-blocking mode: ") +
                ec->message());
        }
        this->setConnectionState(ConnectionState::CONNECTING);
    } catch (const std::exception& e) {
        this->_connected = false;
        throw std::runtime_error(
            std::string("[CLIENT NETWORK] Connection failed: ") + e.what());
    }
}

void net::UnixClientNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            auto ec = std::make_shared<NetworkErrorCode>();
            _socket->close(ec);
            if (*ec) {
                std::cerr << "[CLIENT NETWORK] Warning: Error closing socket: "
                          << ec->message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[CLIENT NETWORK] Warning: Exception closing socket: "
                      << e.what() << std::endl;
        }
    }
    _connected = false;
    _isRunning = false;
}

void net::UnixClientNetwork::disconnect() {
    if (_socket && _socket->isOpen()) {
        try {
            auto ec = std::make_shared<NetworkErrorCode>();
            _socket->close(ec);
            if (*ec) {
                std::cerr << "[CLIENT NETWORK] Warning: Error closing socket: "
                          << ec->message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[CLIENT NETWORK] Warning: Exception closing socket: "
                      << e.what() << std::endl;
        }
    }
    _connected = false;
    this->setConnectionState(ConnectionState::DISCONNECTED);
    if (this->_onDisconnectCallback) {
        this->_onDisconnectCallback(0);
    }
}

bool net::UnixClientNetwork::isConnected() const {
    return this->_connected;
}

bool net::UnixClientNetwork::sendTo(const INetworkEndpoint& endpoint,
    std::vector<uint8_t> packet) {
    try {
        if (!this->_socket || !this->_socket->isOpen()) {
            std::cerr << "[CLIENT NETWORK] Socket is not open for sending." << std::endl;
            return false;
        }
        if (packet.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to send." << std::endl;
            return false;
        }
        auto ec = std::make_shared<NetworkErrorCode>();
        _socket->sendTo(packet, endpoint, 0, ec);
        if (*ec) {
            std::cerr << "[CLIENT NETWORK] Send error: " << ec->message() << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Send error: " << e.what() << std::endl;
        return false;
    }
}

bool net::UnixClientNetwork::broadcast(const std::vector<std::shared_ptr<
    net::INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) {
    if (data.empty()) {
        std::cerr << "[CLIENT NETWORK] No data to broadcast." << std::endl;
        return false;
    }

    try {
        for (const auto &endpoint : endpoints) {
            if (!this->sendTo(*endpoint, data)) {
                std::cerr << "[CLIENT NETWORK] Broadcast error to endpoint: "
                    << endpoint->getAddress() << ":" << endpoint->getPort() << std::endl;
                return false;
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Broadcast error: " << e.what() << std::endl;
        return false;
    }
}

bool net::UnixClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->isOpen()) {
        return false;
    }
    return false;
}


std::vector<uint8_t> net::UnixClientNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;

    auto ec = std::make_shared<NetworkErrorCode>();
    auto buffer = std::make_shared<std::vector<uint8_t>>(65536);
    auto sender = std::make_shared<NetworkEndpoint>();

    std::size_t bytesReceived = _socket->receiveFrom(buffer, sender, 0, ec);

    if (*ec) {
        if (*ec == NetworkError::WOULD_BLOCK || *ec == NetworkError::AGAIN) {
            return std::vector<uint8_t>();
        }
        std::cerr << "[CLIENT NETWORK] Receive error: " << ec->message() << std::endl;
        return std::vector<uint8_t>();
    }

    if (bytesReceived > 0) {
        buffer->resize(bytesReceived);
        this->_serverEndpoint = std::make_shared<NetworkEndpoint>(*sender);
        return *buffer;
    }
    return std::vector<uint8_t>();
}

std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>>
    net::UnixClientNetwork::receiveAny() {
    return std::make_pair(std::make_shared<NetworkEndpoint>(), std::vector<uint8_t>());
}

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
