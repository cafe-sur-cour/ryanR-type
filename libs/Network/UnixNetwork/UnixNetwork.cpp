/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UnixNetwork
*/

#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>

#include "UnixNetwork.hpp"
#include "../../common/DLLoader/LoaderType.hpp"

namespace net {

UnixNetwork::UnixNetwork() : _nextClientId(1), _port(0), _isRunning(false) {
    _ioContext = std::make_shared<asio::io_context>();
}

UnixNetwork::~UnixNetwork() {
    if (_isRunning) {
        stop();
    }
}

void UnixNetwork::init(int port) {
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

void UnixNetwork::stop() {
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _clients.clear();
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

int UnixNetwork::acceptConnection() {
    if (!_socket || !_socket->is_open()) {
        return -1;
    }

    asio::ip::udp::endpoint senderEndpoint;
    std::array<char, 1024> buffer;
    std::error_code ec;

    size_t received = _socket->receive_from(asio::buffer(buffer),
        senderEndpoint, 0, ec);
    if (ec || received == 0) {
        return -1;
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
    std::cout << "[UnixNetwork] New client " << newClientId << " from "
              << senderEndpoint.address().to_string() << ":" <<
              senderEndpoint.port() << std::endl;
    return newClientId;
}

void UnixNetwork::closeConnection(int connectionId) {
    auto it = _clients.find(connectionId);
    if (it != _clients.end()) {
        std::cout << "[UnixNetwork] Closing connection " <<
            connectionId << std::endl;
        _clients.erase(it);

        if (_onDisconnectCallback) {
            _onDisconnectCallback(connectionId);
        }
    }
}

std::vector<int> UnixNetwork::getActiveConnections() const {
    std::vector<int> connections;
    for (const auto& [clientId, endpoint] : _clients) {
        connections.push_back(clientId);
    }
    return connections;
}

size_t UnixNetwork::getConnectionCount() const {
    return _clients.size();
}

void UnixNetwork::sendTo(int connectionId, const pm::IPacketManager &packet) {
    auto it = _clients.find(connectionId);
    if (it == _clients.end()) {
        std::cerr << "[UnixNetwork] Client " << connectionId <<
            " not found" << std::endl;
        return;
    }

    if (!_socket || !_socket->is_open()) {
        std::cerr << "[UnixNetwork] Socket not available" << std::endl;
        return;
    }

    try {
        /* Packet serialization */
        (void)packet;
        std::string data = "Response from server";
        _socket->send_to(asio::buffer(data), it->second);
        std::cout << "[UnixNetwork] Sent data to client " <<
            connectionId << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[UnixNetwork] Error sending to client " <<
            connectionId << ": " << e.what() << std::endl;
    }
}

void UnixNetwork::broadcast(const pm::IPacketManager &packet) {
    for (const auto& [clientId, endpoint] : _clients) {
        sendTo(clientId, packet);
    }
}

bool UnixNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}

std::shared_ptr<pm::IPacketManager> UnixNetwork::receiveFrom(
    const int &connectionId) {
    (void)connectionId;
    return nullptr;
}

void UnixNetwork::sendData(const pm::IPacketManager &data, size_t size) {
    (void)size;
    broadcast(data);
}
pm::IPacketManager &UnixNetwork::receiveData(
    const IBuffer &buffer, size_t size) const {
    (void)buffer;  // To avoid unused parameter warning
    (void)size;   // To avoid unused parameter warning
    throw std::runtime_error("[UnixNetwork] receiveData not implemented");
}

void UnixNetwork::setConnectionCallback(std::function<void(int)> onConnect) {
    _onConnectCallback = onConnect;
}

void UnixNetwork::setDisconnectionCallback(std::function<void(int)>
    onDisconnect) {
    _onDisconnectCallback = onDisconnect;
}

}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixNetwork();
    }
    int getType() {
        return NETWORK_MODULE;
    }
}
