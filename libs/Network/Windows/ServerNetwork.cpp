/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerNetwork
*/

#include <iostream>
#include <iomanip>
#include <queue>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "ServerNetwork.hpp"
#include "../common.hpp"
#include "../INetworkSocket.hpp"
#include "../INetworkResolver.hpp"
#include "../INetworkErrorCode.hpp"
#include "../INetworkAddress.hpp"
#include "../INetworkEndpoint.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"
#include "../../Packet/PacketManager.hpp"

net::ServerNetwork::ServerNetwork() : _nextClientId(0), _port(0) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;

    this->_incomingPackets = {};
    this->_clients = {};
}

net::ServerNetwork::~ServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void net::ServerNetwork::init(uint16_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<NetworkSocket>(_eventLoop);
    NetworkErrorCode ec;
    if (!_socket->open(ec)) {
        throw std::runtime_error(std::string(
            "[SERVER NETWORK] Failed to open socket: ") + ec.message());
    }

    NetworkAddress bindAddress;
    if (host.empty()) {
        bindAddress = NetworkAddress::anyV4();
    } else {
        bindAddress = NetworkAddress::fromString(host);
        if (bindAddress.toString().empty()) {
            auto resolver = std::make_shared<NetworkResolver>(_eventLoop);
            auto results = resolver->resolve(host, std::to_string(_port), ec);
            if (ec || results.empty()) {
                throw std::runtime_error(
                    std::string("[SERVER NETWORK] Failed to resolve host '")
                    + host + "': " + (ec.hasError() ? ec.message() : "no results"));
            }
            bindAddress = NetworkAddress::fromString(results[0]->getAddress());
        }
    }

    NetworkEndpoint bindEndpoint(bindAddress, _port);
    if (!_socket->bind(bindEndpoint, ec)) {
        throw std::runtime_error(
            std::string("[SERVER NETWORK] Failed to bind socket: ") + ec.message());
    }

    if (!_socket->setNonBlocking(true, ec)) {
        throw std::runtime_error(
            std::string("[SERVER NETWORK] Failed to set non-blocking: ") + ec.message());
    }

    _isRunning = true;
}

void net::ServerNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            NetworkErrorCode ec;
            _socket->close(ec);
            if (ec) {
                std::cerr << "[SERVER NETWORK] Warning: Error closing socket: "
                          << ec.message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[SERVER NETWORK] Warning: Exception closing socket: "
                      << e.what() << std::endl;
        }
    }
    _clients.clear();
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool net::ServerNetwork::sendTo(const net::INetworkEndpoint& endpoint,
    std::vector<uint8_t> packet) {
    if (!_socket || !_socket->isOpen()) {
        std::cerr << "[SERVER NETWORK] Socket is not open" << std::endl;
        return false;
    }

    NetworkErrorCode ec;
    _socket->sendTo(packet, endpoint, 0, ec);
    if (ec) {
        std::cerr << "[SERVER NETWORK] Send error: " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool net::ServerNetwork::broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>
    >& endpoints, const std::vector<uint8_t>& data) {
    if (data.empty()) {
        std::cerr << "[SERVER NETWORK] No data to broadcast." << std::endl;
        return false;
    }

    for (const auto &endpoint : endpoints) {
        if (!this->sendTo(*endpoint, data)) {
            std::cerr << "[SERVER NETWORK] Broadcast error to endpoint: "
                << endpoint->getAddress() << ":" << endpoint->getPort() << std::endl;
            return false;
        }
    }
    return true;
}

bool net::ServerNetwork::hasIncomingData() const {
    if (!_socket || !_socket->isOpen()) {
        return false;
    }
    return false;
}

std::vector<uint8_t> net::ServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return std::vector<uint8_t>();
}

std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>>
    net::ServerNetwork::receiveAny() {
    NetworkErrorCode ec;

    std::vector<uint8_t> buffer(65536);
    NetworkEndpoint sender;
    std::size_t bytes = _socket->receiveFrom(buffer, sender, 0, ec);
    if (ec) {
        if (ec == net::NetworkError::WOULD_BLOCK || ec == net::NetworkError::AGAIN) {
            return {};
        }
        std::cerr << "[SERVER NETWORK] Receive error: " << ec.message() << std::endl;
        return {};
    }

    buffer.resize(bytes);
    return std::make_pair(std::make_shared<NetworkEndpoint>(sender), buffer);
}

extern "C" {
    void *createNetworkInstance() {
        return new net::ServerNetwork();
    }
    int getType() {
        return NETWORK_SERVER_MODULE;
    }
}

