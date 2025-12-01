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

void ServerNetwork::init(uint32_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
    _socket->open(asio::ip::udp::v4());
    if (port < 1024 || port > 65535) {
        throw std::invalid_argument("Port must be between 1024 and 65535");
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

uint8_t ServerNetwork::acceptConnection() {
    if (!_socket || !_socket->is_open()) {
        return 1;
    }

    asio::ip::udp::endpoint senderEndpoint;
    std::array<char, 1024> buffer;
    std::error_code ec;

    size_t received = _socket->receive_from(asio::buffer(buffer),
        senderEndpoint, 0, ec);
    if (ec || received == 0) {
        return 1;
    }
    for (const auto& [clientId, endpoint] : _clients) {
        if (endpoint == senderEndpoint) {
            //* Add Packet */
            return clientId;
        }
    }

    int newClientId = _nextClientId++;
    _clients[newClientId] = senderEndpoint;
    if (_onConnectCallback) {
        _onConnectCallback(newClientId);
    }
    std::cout << "[ServerNetwork] New client " << newClientId << " from "
              << senderEndpoint.address().to_string() << ":" <<
              senderEndpoint.port() << std::endl;
    return newClientId;
}

void ServerNetwork::sendTo(int connectionId, const pm::IPacketManager &packet) {
    auto it = _clients.find(connectionId);
    if (it == _clients.end()) {
        std::cerr << "[ServerNetwork] Client " << connectionId <<
            " not found" << std::endl;
        return;
    }

    if (!_socket || !_socket->is_open()) {
        std::cerr << "[ServerNetwork] Socket not available" << std::endl;
        return;
    }

    try {
        /* Packet serialization */
        (void)packet;
        std::string data = "Response from server";
        _socket->send_to(asio::buffer(data), it->second);
        std::cout << "[ServerNetwork] Sent data to client " <<
            connectionId << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ServerNetwork] Error sending to client " <<
            connectionId << ": " << e.what() << std::endl;
    }
}

void ServerNetwork::broadcast(const pm::IPacketManager &packet) {
    for (const auto& [clientId, endpoint] : _clients) {
        sendTo(clientId, packet);
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
    const int &connectionId) {
    (void)connectionId;
    return nullptr;
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

