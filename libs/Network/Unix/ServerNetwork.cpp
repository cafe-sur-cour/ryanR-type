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
#include "../INetworkSocket.hpp"
#include "../INetworkResolver.hpp"
#include "../INetworkErrorCode.hpp"
#include "../INetworkAddress.hpp"
#include "../Asio/Socket.hpp"
#include "../Asio/Resolver.hpp"
#include "../Asio/ErrorCode.hpp"
#include "../Asio/Address.hpp"
#include "../Asio/Endpoint.hpp"
#include "../Asio/EventLoop.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"
#include "../../Packet/PacketManager.hpp"

namespace net {

UnixServerNetwork::UnixServerNetwork() : _port(0) {
    _eventLoop = EventLoopFactory::create();
    _socket = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;

    this->_incomingPackets = {};
}

UnixServerNetwork::~UnixServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void UnixServerNetwork::init(uint16_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<AsioSocket>(_eventLoop);
    AsioErrorCode ec;
    if (!_socket->open(ec)) {
        throw std::runtime_error(std::string(
            "[SERVER NETWORK] Failed to open socket: ") + ec.message());
    }

    AsioAddress bindAddress;
    if (host.empty()) {
        bindAddress = AsioAddress::anyV4();
    } else {
        bindAddress = AsioAddress::fromString(host);
        if (bindAddress.toString().empty()) {
            auto resolver = std::make_shared<AsioResolver>(_eventLoop);
            auto results = resolver->resolve(host, std::to_string(_port), ec);
            if (ec || results.empty()) {
                throw std::runtime_error(
                    std::string("[SERVER NETWORK] Failed to resolve host '")
                    + host + "': " + (ec.hasError() ? ec.message() : "no results"));
            }
            bindAddress = AsioAddress::fromString(results[0].getAddress());
        }
    }

    AsioEndpoint bindEndpoint(bindAddress, _port);
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

void UnixServerNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            AsioErrorCode ec;
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
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool UnixServerNetwork::sendTo(const INetworkEndpoint& endpoint, std::vector<uint8_t> packet) {
    if (!_socket || !_socket->isOpen()) {
        std::cerr << "[SERVER NETWORK] Socket is not open" << std::endl;
        return false;
    }

    AsioErrorCode ec;
    _socket->sendTo(packet, endpoint, 0, ec);
    if (ec) {
        std::cerr << "[SERVER NETWORK] Send error: " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool UnixServerNetwork::broadcast(const std::vector<std::shared_ptr<INetworkEndpoint>>& endpoints,
    const std::vector<uint8_t>& data) {
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

bool UnixServerNetwork::hasIncomingData() const {
    if (_incomingPackets.empty())
        return false;
    return true;
}


std::vector<uint8_t> UnixServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return std::vector<uint8_t>();
}

std::pair<std::shared_ptr<INetworkEndpoint>, std::vector<uint8_t>> UnixServerNetwork::receiveAny() {
    AsioErrorCode ec;

    std::vector<uint8_t> buffer(65536);
    AsioEndpoint sender;
    std::size_t bytes = _socket->receiveFrom(buffer, sender, 0, ec);
    if (ec) {
        if (ec == NetworkError::WOULD_BLOCK || ec == NetworkError::AGAIN) {
            return {};
        }
        std::cerr << "[SERVER NETWORK] Receive error: " << ec.message() << std::endl;
        return {};
    }

    buffer.resize(bytes);
    return std::make_pair(std::make_shared<AsioEndpoint>(sender), buffer);
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixServerNetwork();
    }
    int getType() {
        return NETWORK_SERVER_MODULE;
    }
}

