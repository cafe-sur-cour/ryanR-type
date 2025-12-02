/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerNetwork
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

uint8_t ServerNetwork::acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) {
    (void)packetManager;
    if (!_socket || !_socket->is_open()) {
        return 0;
    }

    for (const auto& [clientId, endpoint] : _clients) {
        if (endpoint == id) {
            //* Add Packet */
            return clientId;
        }
    }

    uint8_t newClientId = _nextClientId++;
    _clients[newClientId] = id;
    if (_onConnectCallback) {
        _onConnectCallback(newClientId);
    }
    std::cout << "[ServerNetwork] New client " << static_cast<int>(newClientId) << " from "
              << id.address().to_string() << ":"
              << id.port() << std::endl;
    return newClientId;
}

void ServerNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    if (!_socket || !_socket->is_open()) {
        std::cerr << "[ServerNetwork] Socket not available" << std::endl;
        return;
    }

    try {
        _socket->send_to(asio::buffer(packet), id);
        std::cout << "[ServerNetwork] Sent data to " <<
            id.address().to_string() << ":" << id.port() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ServerNetwork] Error sending to endpoint: " << e.what() << std::endl;
    }
}

void ServerNetwork::broadcast(const pm::IPacketManager &packet) {
    std::vector<uint8_t> data = packet.pack(0, 0, packet.getType());
    for (const auto& [clientId, endpoint] : _clients) {
        (void)clientId;
        sendTo(endpoint, data);
    }
}

bool ServerNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}

std::shared_ptr<pm::IPacketManager> ServerNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    return nullptr;
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

