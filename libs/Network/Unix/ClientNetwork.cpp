/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** UnixClientNetwork
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

#include "ClientNetwork.hpp"
#include "../../../common/DLLoader/LoaderType.hpp"

namespace net {

UnixClientNetwork::UnixClientNetwork() : _connected(false) {
    _ioContext = std::make_shared<asio::io_context>();
    _isRunning = false;
}

UnixClientNetwork::~UnixClientNetwork() {
    if (_connected) {
        disconnect();
    }
}

void UnixClientNetwork::init(uint32_t port, const std::string host) {
    if (!_socket) {
        _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
    }

    if (!_socket->is_open()) {
        _socket->open(asio::ip::udp::v4());
    }
    std::cout << "Socket is open" << std::endl;
    try {
        asio::ip::udp::resolver resolver(*_ioContext);
        asio::ip::udp::resolver::results_type endpoints =
            resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));

        _serverEndpoint = *endpoints.begin();
        this->setConnectionState(ConnectionState::CONNECTING);
        // this->_socket->non_blocking(true);
    } catch (const std::exception& e) {
        _connected = false;
        throw std::runtime_error(std::string
            ("[UnixClientNetwork] Connection failed: ") + e.what());
    }
}

void UnixClientNetwork::stop() {
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _connected = false;
    _isRunning = false;
}

void UnixClientNetwork::disconnect() {
}

bool UnixClientNetwork::isConnected() const {
    return _connected;
}

uint8_t UnixClientNetwork::acceptConnection() {
    // Not applicable for client
    return 1;
}

void UnixClientNetwork::sendTo(int connectionId, std::vector<uint8_t> packet) {
   (void)connectionId;  // In client, connectionId is not used
   std::cout << "Sending to " << _serverEndpoint.address().to_string() << ":"
             << _serverEndpoint.port() << " Data size: " << packet.size() << std::endl;
    _socket->send_to(asio::buffer(packet), _serverEndpoint);
}

void UnixClientNetwork::broadcast(std::vector<uint8_t> packet) {
    // For client, broadcast is the same as sending to the server
    sendTo(0, packet);
}

bool UnixClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}

std::shared_ptr<pm::IPacketManager> UnixClientNetwork::receiveFrom(
    const int &connectionId) {
    (void)connectionId;
    return nullptr;
}


}  // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
