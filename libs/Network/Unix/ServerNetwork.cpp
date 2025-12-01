/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixServerNetwork
*/

#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

#include "ServerNetwork.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

UnixServerNetwork::UnixServerNetwork() : _nextClientId(1), _port(0) {
    _ioContext = std::make_shared<asio::io_context>();
    _isRunning = false;
}

UnixServerNetwork::~UnixServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void UnixServerNetwork::init(uint16_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);

    std::error_code ec;
    _socket->open(asio::ip::udp::v4(), ec);
    if (ec) {
        throw std::runtime_error(std::string(
            "[UnixServerNetwork] Failed to open socket: ") + ec.message());
    }

    asio::ip::address bindAddress;
    if (host.empty()) {
        bindAddress = asio::ip::address_v4::any();
    } else {
        try {
            bindAddress = asio::ip::make_address(host);
        } catch (const std::exception &) {
            asio::ip::udp::resolver resolver(*_ioContext);
            auto results = resolver.resolve(host, std::to_string(_port), ec);
            if (ec || results.empty()) {
                throw std::runtime_error(
                    std::string("[UnixServerNetwork] Failed to resolve host '")
                    + host + "': " + (ec ? ec.message() : "no results"));
            }
            bindAddress = results.begin()->endpoint().address();
        }
    }

    _socket->bind(asio::ip::udp::endpoint(bindAddress, _port), ec);
    if (ec) {
        throw std::runtime_error(
            std::string("[UnixServerNetwork] Failed to bind socket: ") + ec.message());
    }
    std::cout << "[UnixServerNetwork] Server started on " <<
        bindAddress.to_string() << ":" << _port << std::endl;
    _isRunning = true;
}

void UnixServerNetwork::stop() {
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _clients.clear();
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

uint8_t UnixServerNetwork::acceptConnection() {
    if (!_socket || !_socket->is_open()) {
        return 0;
    }

    asio::ip::udp::endpoint senderEndpoint;
    std::array<char, 1024> buffer;  // Temporary buffer for receiving data
    std::error_code ec;

    size_t received = _socket->receive_from(asio::buffer(buffer),
        senderEndpoint, 0, ec);
    if (ec || received == 0) {
        return 0;
    }

    for (const auto& [clientId, endpoint] : _clients) {
        if (endpoint == senderEndpoint) {
            return clientId;
        }
    }

    uint8_t newClientId = _nextClientId++;
    _clients[newClientId] = senderEndpoint;
    if (_onConnectCallback) {
        _onConnectCallback(newClientId);
    }
    std::cout << "[UnixServerNetwork] New client " << newClientId << " from "
              << senderEndpoint.address().to_string() << ":" <<
              senderEndpoint.port() << std::endl;
    return newClientId;
}

void UnixServerNetwork::sendTo(uint8_t connectionId, const pm::IPacketManager &packet) {
    (void)packet;
    (void)connectionId;
}

void UnixServerNetwork::broadcast(const pm::IPacketManager &packet) {
    (void)packet;
}

bool UnixServerNetwork::hasIncomingData() const {
    return false;
}

std::shared_ptr<pm::IPacketManager> UnixServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return nullptr;
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

