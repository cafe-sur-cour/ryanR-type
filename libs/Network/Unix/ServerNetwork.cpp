/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixServerNetwork
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
#include "../../../common/interfaces/INetworkSocket.hpp"
#include "../../../common/interfaces/INetworkResolver.hpp"
#include "../../../common/interfaces/INetworkErrorCode.hpp"
#include "../../../common/interfaces/INetworkAddress.hpp"
#include "../../../common/interfaces/INetworkEndpoint.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"
#include "../../Packet/PacketManager.hpp"

net::UnixServerNetwork::UnixServerNetwork() : _port(0) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;

    this->_incomingPackets = {};
}

net::UnixServerNetwork::~UnixServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void net::UnixServerNetwork::init(uint16_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<NetworkSocket>(_eventLoop);
    auto ec = std::make_shared<NetworkErrorCode>();
    if (!_socket->open(ec)) {
        throw std::runtime_error(std::string(
            "[SERVER NETWORK] Failed to open socket: ") + ec->message());
    }

    NetworkAddress bindAddress;
    if (host.empty()) {
        bindAddress = NetworkAddress::anyV4();
    } else {
        bindAddress = NetworkAddress::fromString(host);
        if (bindAddress.toString().empty()) {
            auto resolver = std::make_shared<NetworkResolver>(_eventLoop);
            auto results = resolver->resolve(host, std::to_string(_port), ec);
            if (*ec || results.empty()) {
                throw std::runtime_error(
                    std::string("[SERVER NETWORK] Failed to resolve host '")
                    + host + "': " + (ec->hasError() ? ec->message() : "no results"));
            }
            bindAddress = NetworkAddress::fromString(results[0]->getAddress());
        }
    }

    NetworkEndpoint bindEndpoint(bindAddress, _port);
    if (!_socket->bind(bindEndpoint, ec)) {
        throw std::runtime_error(
            std::string("[SERVER NETWORK] Failed to bind socket: ") + ec->message());
    }

    if (!_socket->setNonBlocking(true, ec)) {
        throw std::runtime_error(
            std::string("[SERVER NETWORK] Failed to set non-blocking: ") + ec->message());
    }

    _isRunning = true;
}

void net::UnixServerNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            auto ec = std::make_shared<NetworkErrorCode>();
            _socket->close(ec);
            if (*ec) {
                std::cerr << "[SERVER NETWORK] Warning: Error closing socket: "
                          << ec->message() << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[SERVER NETWORK] Warning: Exception closing socket: "
                      << e.what() << std::endl;
        }
    }
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool net::UnixServerNetwork::sendTo(const INetworkEndpoint& endpoint,
    std::vector<uint8_t> packet) {
    if (!_socket || !_socket->isOpen()) {
        std::cerr << "[SERVER NETWORK] Socket is not open" << std::endl;
        return false;
    }

    auto ec = std::make_shared<NetworkErrorCode>();
    _socket->sendTo(packet, endpoint, 0, ec);
    if (*ec) {
        std::cerr << "[SERVER NETWORK] Send error: " << ec->message() << std::endl;
        return false;
    }
    return true;
}

bool net::UnixServerNetwork::broadcast(const std::vector<std::shared_ptr<
    net::INetworkEndpoint>>& endpoints, const std::vector<uint8_t>& data) {
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

bool net::UnixServerNetwork::hasIncomingData() const {
    if (_incomingPackets.empty())
        return false;
    return true;
}


std::vector<uint8_t> net::UnixServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return std::vector<uint8_t>();
}

std::pair<std::shared_ptr<net::INetworkEndpoint>, std::vector<uint8_t>>
    net::UnixServerNetwork::receiveAny() {
    auto ec = std::make_shared<NetworkErrorCode>();

    auto buffer = std::make_shared<std::vector<uint8_t>>(MAX_UDP_PACKET_SIZE);
    auto sender = std::make_shared<NetworkEndpoint>();
    std::size_t bytes = _socket->receiveFrom(buffer, sender, 0, ec);
    if (*ec) {
        if (*ec == NetworkError::WOULD_BLOCK || *ec == NetworkError::AGAIN) {
            return {};
        }
        std::cerr << "[SERVER NETWORK] Receive error: " << ec->message() << std::endl;
        return {};
    }

    buffer->resize(bytes);
    return std::make_pair(std::make_shared<NetworkEndpoint>(*sender), *buffer);
}

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixServerNetwork();
    }
    int getType() {
        return NETWORK_SERVER_MODULE;
    }
}

