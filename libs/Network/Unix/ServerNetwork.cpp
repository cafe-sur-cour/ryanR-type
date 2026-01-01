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
#include "../NetworkSocket.hpp"
#include "../NetworkResolver.hpp"
#include "../NetworkErrorCode.hpp"
#include "../NetworkAddress.hpp"
#include "../AsioEventLoop.hpp"
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
    _socket = NetworkSocketFactory::createUdpSocket(_eventLoop);
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
            auto resolver = NetworkResolverFactory::createUdpResolver(_eventLoop);
            auto results = resolver->resolve(host, std::to_string(_port), ec);
            if (ec || results.empty()) {
                throw std::runtime_error(
                    std::string("[SERVER NETWORK] Failed to resolve host '")
                    + host + "': " + (ec.hasError() ? ec.message() : "no results"));
            }
            bindAddress = NetworkAddress::fromString(results[0].getAddress());
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

void UnixServerNetwork::stop() {
    if (_socket && _socket->isOpen()) {
        try {
            NetworkErrorCode ec;
            _socket->close(ec);
        } catch (const std::exception&) {
        }
    }
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool UnixServerNetwork::sendTo(const NetworkEndpoint& endpoint, std::vector<uint8_t> packet) {
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

bool UnixServerNetwork::broadcast(const std::vector<NetworkEndpoint>& endpoints,
    std::vector<uint8_t> data) {
    for (const auto &endpoint : endpoints) {
        if (!this->sendTo(endpoint, data)) {
            std::cerr << "[SERVER NETWORK] Broadcast error to endpoint: "
                << endpoint.getAddress() << ":" << endpoint.getPort() << std::endl;
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

std::pair<NetworkEndpoint, std::vector<uint8_t>> UnixServerNetwork::receiveAny() {
    NetworkErrorCode ec;

    std::vector<uint8_t> buffer(65536);
    NetworkEndpoint sender;
    std::size_t bytes = _socket->receiveFrom(buffer, sender, 0, ec);
    if (ec) {
        if (ec == NetworkError::WOULD_BLOCK || ec == NetworkError::AGAIN) {
            return {};
        }
        std::cerr << "[SERVER NETWORK] Receive error: " << ec.message() << std::endl;
        return {};
    }

    buffer.resize(bytes);
    return std::make_pair(sender, buffer);
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

