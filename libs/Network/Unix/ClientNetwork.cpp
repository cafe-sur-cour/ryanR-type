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

void UnixClientNetwork::init(int port) {
    (void)port;
    //  connect to exisiting server
}

void UnixClientNetwork::stop() {
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _connected = false;
    _isRunning = false;
}

void UnixClientNetwork::connect(const std::string &host, int port) {
    if (!_socket || !_socket->is_open()) {
        throw std::runtime_error("[UnixClientNetwork] Socket not initialized.");
    }

    try {
        asio::ip::udp::resolver resolver(*_ioContext);
        asio::ip::udp::resolver::results_type endpoints =
            resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));

        _serverEndpoint = *endpoints.begin();
        _connected = true;

        std::cout << "[UnixClientNetwork] Connected to server " << host
                  << ":" << port << std::endl;

        std::string connectMsg = "CLIENT_CONNECT";
        _socket->send_to(asio::buffer(connectMsg), _serverEndpoint);
    } catch (const std::exception& e) {
        _connected = false;
        throw std::runtime_error(std::string
            ("[UnixClientNetwork] Connection failed: ") + e.what());
    }
}

void UnixClientNetwork::disconnect() {
    if (_connected && _socket && _socket->is_open()) {
        try {
            std::string disconnectMsg = "CLIENT_DISCONNECT";
            _socket->send_to(asio::buffer(disconnectMsg), _serverEndpoint);
            std::cout << "[UnixClientNetwork] Disconnected from server"
                << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[UnixClientNetwork] Error during disconnect: "
            << e.what() << std::endl;
        }
    }
    _connected = false;
}

bool UnixClientNetwork::isConnected() const {
    return _connected;
}

int UnixClientNetwork::acceptConnection() {
    // Not applicable for client
    return -1;
}

void UnixClientNetwork::sendTo(int connectionId, const pm::IPacketManager &packet) {
    (void)connectionId;

    if (!_connected) {
        std::cerr << "[UnixClientNetwork] Not connected to server" << std::endl;
        return;
    }
    if (!_socket || !_socket->is_open()) {
        std::cerr << "[UnixClientNetwork] Socket not available" << std::endl;
        return;
    }

    try {
        /* Packet serialization */
        (void)packet;
        std::string data = "Client message";
        _socket->send_to(asio::buffer(data), _serverEndpoint);
        std::cout << "[UnixClientNetwork] Sent data to server" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[UnixClientNetwork] Error sending data: "
            << e.what() << std::endl;
    }
}

void UnixClientNetwork::broadcast(const pm::IPacketManager &packet) {
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

    if (!_connected) {
        return nullptr;
    }

    if (!hasIncomingData()) {
        return nullptr;
    }

    try {
        std::array<char, 1024> buffer;
        asio::ip::udp::endpoint senderEndpoint;
        std::error_code ec;

        size_t received = _socket->receive_from(asio::buffer(buffer),
            senderEndpoint, 0, ec);
        if (ec || received == 0) {
            return nullptr;
        }

        std::cout << "[UnixClientNetwork] Received " << received
                  << " bytes from server" << std::endl;

        return nullptr;
    } catch (const std::exception& e) {
        std::cerr << "[UnixClientNetwork] Error receiving data: "
                  << e.what() << std::endl;
        return nullptr;
    }
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
