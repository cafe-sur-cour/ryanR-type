/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientNetwork
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "ClientNetwork.hpp"
#include "../INetworkSocket.hpp"
#include "../INetworkResolver.hpp"
#include "../INetworkErrorCode.hpp"
#include "../Asio/AsioSocket.hpp"
#include "../Asio/AsioResolver.hpp"
#include "../Asio/AsioErrorCode.hpp"
#include "../Asio/AsioEndpoint.hpp"
#include "../Asio/AsioEventLoop.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

ClientNetwork::ClientNetwork() : _connected(false) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _serverEndpoint = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;
}

ClientNetwork::~ClientNetwork() {
    if (_connected) {
        disconnect();
    }
}


void ClientNetwork::init(uint16_t port, const std::string host) {
    AsioErrorCode ec;

    if (!this->_socket) {
        this->_socket = std::make_shared<AsioSocket>(_eventLoop);
    }

    if (!this->_socket->isOpen()) {
        if (!this->_socket->open(ec)) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to open socket: ") + ec.message());
        }
    }
    try {
        auto resolver = std::make_shared<AsioResolver>(_eventLoop);
        auto endpoints = resolver->resolve(host, std::to_string(port), ec);
        if (ec || endpoints.empty()) {
            throw std::runtime_error(
                "[CLIENT NETWORK] Failed to resolve host '" + host + "': " + 
                (ec.hasError() ? ec.message() : "no results"));
        }
        this->_serverEndpoint = std::make_shared<AsioEndpoint>(endpoints[0]);
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

void ClientNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            AsioErrorCode ec;
            _socket->close(ec);
            if (ec) {
                std::cerr << "[CLIENT NETWORK] Warning: Error closing socket: " 
                          << ec.message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[CLIENT NETWORK] Warning: Exception closing socket: " 
                      << e.what() << std::endl;
        }
    }
    _connected = false;
    _isRunning = false;
}

void ClientNetwork::disconnect() {
    if (_socket && _socket->isOpen()) {
        try {
            AsioErrorCode ec;
            _socket->close(ec);
            if (ec) {
                std::cerr << "[CLIENT NETWORK] Warning: Error closing socket: " 
                          << ec.message() << std::endl;
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

bool ClientNetwork::isConnected() const {
    return this->_connected;
}

bool ClientNetwork::sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) {
    try {
        if (!this->_socket || !this->_socket->isOpen()) {
            std::cerr << "[CLIENT NETWORK] Socket is not open for sending." << std::endl;
            return false;
        }
        if (packet.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to send." << std::endl;
            return false;
        }
        AsioErrorCode ec;
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

bool ClientNetwork::broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints,
    const std::vector<uint8_t>& data) {
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

bool ClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->isOpen()) {
        return false;
    }
    // En mode non-bloquant, on peut tenter une lecture pour vérifier
    // Pour simplifier, on retourne toujours false ici et on gère dans receiveFrom
    return false;
}


std::vector<uint8_t> ClientNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;

    AsioErrorCode ec;
    std::vector<uint8_t> buffer(65536); // Taille maximale UDP
    AsioEndpoint sender;

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
        this->_serverEndpoint = std::make_shared<AsioEndpoint>(sender);
        return buffer;
    }
    return std::vector<uint8_t>();
}

std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> ClientNetwork::receiveAny() {
    // TODO: Implement if needed for client
    return std::make_pair(std::make_shared<AsioEndpoint>(), std::vector<uint8_t>());
}

}  //  namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::ClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
