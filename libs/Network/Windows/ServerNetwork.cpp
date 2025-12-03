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
#include "../../../common/DLLoader/LoaderType.hpp"
#include "../../Packet/PacketManager.hpp"

namespace net {

ServerNetwork::ServerNetwork() : _nextClientId(1), _port(0) {
    _ioContext = std::make_shared<asio::io_context>();
    _isRunning = false;
}

ServerNetwork::~ServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void ServerNetwork::init(uint16_t port, const std::string host) {
    (void)host;
    _port = port;
    _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
    _socket->open(asio::ip::udp::v4());
    if (port < 0 || port > 65535) {
        throw std::invalid_argument("Port must be between 0 and 65535");
    }

    _socket->bind(asio::ip::udp::endpoint(asio::ip::udp::v4(),
        static_cast<asio::ip::port_type>(port)));

    _isRunning = true;
}

void ServerNetwork::stop() {
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _clients.clear();
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool ServerNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    (void)id;
    (void)packet;
    return false;
}

void ServerNetwork::broadcast(std::vector<uint8_t> data) {
    (void)data;
}

bool ServerNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}

std::vector<uint8_t> ServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return std::vector<uint8_t>();
}

std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> ServerNetwork::receiveAny() {
    if (!_socket || !_socket->is_open()) {
        return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
    }

    asio::ip::udp::endpoint senderEndpoint;
    std::vector<uint8_t> buffer(1024);
    std::error_code ec;

    size_t received = _socket->receive_from(asio::buffer(buffer), senderEndpoint, 0, ec);
    if (ec || received == 0) {
        return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
    }

    buffer.resize(received);
    return std::make_pair(senderEndpoint, buffer);
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::ServerNetwork();
    }
    int getType() {
        return NETWORK_SERVER_MODULE;
    }
}

