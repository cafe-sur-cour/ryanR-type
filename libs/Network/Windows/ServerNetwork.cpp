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
    _socket = nullptr;
    _onConnectCallback = nullptr;
    _onDisconnectCallback = nullptr;
    _isRunning = false;
    _connectionState = ConnectionState::DISCONNECTED;

    this->_incomingPackets = {};
    this->_clients = {};
}

ServerNetwork::~ServerNetwork() {
    if (_isRunning) {
        stop();
    }
}

void ServerNetwork::init(uint16_t port, const std::string host) {
    _port = port;
    _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);

    std::error_code ec;
    _socket->open(asio::ip::udp::v4(), ec);
    if (ec) {
        throw std::runtime_error(std::string(
            "[SERVER NETWORK] Failed to open socket: ") + ec.message());
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
                    std::string("[SERVER NETWORK] Failed to resolve host '")
                    + host + "': " + (ec ? ec.message() : "no results"));
            }
            bindAddress = results.begin()->endpoint().address();
        }
    }

    _socket->bind(asio::ip::udp::endpoint(bindAddress, _port), ec);
    if (ec) {
        throw std::runtime_error(
            std::string("[SERVER NETWORK] Failed to bind socket: ") + ec.message());
    }
    _socket->non_blocking(true, ec);
    _isRunning = true;
}

void ServerNetwork::stop() {
    if (_socket && _socket->is_open()) {
        try {
            _socket->close();
        } catch (const std::system_error&) {
        }
    }
    _clients.clear();
    while (!_incomingPackets.empty()) {
        _incomingPackets.pop();
    }
    _isRunning = false;
}

bool ServerNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    if (!_socket || !_socket->is_open()) {
        std::cerr << "[SERVER NETWORK] Socket is not open" << std::endl;
        return false;
    }

    asio::error_code ec;
    _socket->send_to(asio::buffer(packet), id, 0, ec);
    if (ec) {
        std::cerr << "[SERVER NETWORK] Send error: " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool ServerNetwork::broadcast(std::vector<asio::ip::udp::endpoint> clients,
    std::vector<uint8_t> data) {
    for (auto &endpoint : clients) {
        if (!this->sendTo(endpoint, data)) {
            std::cerr << "[SERVER NETWORK] Broadcast error to endpoint: "
                << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;
            return false;
        }
    }
    return true;
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
    asio::error_code ec;

    std::size_t available = _socket->available(ec);
    if (ec) {
        std::cerr << "[SERVER NETWORK] Available check error: " << ec.message() << std::endl;
        return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
    }
    if (available == 0) {
        return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
    }

    std::vector<uint8_t> buffer(available);
    asio::ip::udp::endpoint sender;
    std::size_t bytes = _socket->receive_from(asio::buffer(buffer), sender, 0, ec);
    if (ec) {
        std::cerr << "[SERVER NETWORK] Receive error: " << ec.message() << std::endl;
        return std::make_pair(asio::ip::udp::endpoint(), std::vector<uint8_t>());
    }

    buffer.resize(bytes);
    return std::make_pair(sender, buffer);
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

