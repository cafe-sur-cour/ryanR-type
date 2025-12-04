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
#include <vector>
#include <utility>

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


void UnixClientNetwork::init(uint16_t port, const std::string host) {
    std::error_code ec;

    if (!this->_socket) {
        this->_socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
    }

    if (!this->_socket->is_open()) {
        this->_socket->open(asio::ip::udp::v4(), ec);
        if (ec) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to open socket: ") + ec.message());
        }
    }
    try {
        asio::ip::udp::resolver resolver(*_ioContext);
        asio::ip::udp::resolver::results_type endpoints =
            resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));
        if (endpoints.empty()) {
            throw std::runtime_error(
                "[CLIENT NETWORK] Failed to resolve host '" + host + "': no results");
        }
        this->_serverEndpoint = *endpoints.begin();
        this->_socket->non_blocking(true, ec);
        if (ec) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to set non-blocking mode: ") +
                ec.message());
        }
        this->setConnectionState(ConnectionState::CONNECTING);
    } catch (const std::exception& e) {
        this->_connected = false;
        throw std::runtime_error(
            std::string("[CLIENT NETWORK] Connection failed: ") + e.what());
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
    if (_socket && _socket->is_open()) {
        _socket->close();
    }
    _connected = false;
    this->setConnectionState(ConnectionState::DISCONNECTED);
    if (this->_onDisconnectCallback) {
        this->_onDisconnectCallback(0);
    }
}

bool UnixClientNetwork::isConnected() const {
    return this->_connected;
}

bool UnixClientNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
    (void)id;
    try {
        if (!this->_socket || !this->_socket->is_open()) {
            std::cerr << "[CLIENT NETWORK] Socket is not open for sending." << std::endl;
            return false;
        }
        if (packet.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to send." << std::endl;
            return false;
        }
        _socket->send_to(asio::buffer(packet), this->_serverEndpoint);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Send error: " << e.what() << std::endl;
        return false;
    }
}

void UnixClientNetwork::broadcast(std::vector<uint8_t> data) {
    try {
        if (!_socket || !_socket->is_open()) {
            std::cerr << "[CLIENT NETWORK] Socket is not open for broadcasting." << std::endl;
            return;
        }
        if (data.empty()) {
            std::cerr << "[CLIENT NETWORK] No data to broadcast." << std::endl;
            return;
        }
        _socket->send_to(asio::buffer(data), this->_serverEndpoint);
        std::cout << "[CLIENT NETWORK] Broadcasted " << data.size() << " bytes." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[CLIENT NETWORK] Broadcast error: " << e.what() << std::endl;
    }
}

bool UnixClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}


std::vector<uint8_t> UnixClientNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;

    asio::error_code ec;
    std::size_t available = _socket->available(ec);

    if (ec) {
        std::cerr << "[CLIENT NETWORK] Error checking available data: "
                  << ec.message() << std::endl;
        return std::vector<uint8_t>();
    }

    if (available == 0) {
        return std::vector<uint8_t>();
    }
    std::vector<uint8_t> buffer(available);
    asio::ip::udp::endpoint senderEndpoint;

    std::size_t bytesReceived = _socket->receive_from(
        asio::buffer(buffer), senderEndpoint, 0, ec);

    if (ec) {
        if (ec == asio::error::would_block || ec == asio::error::try_again) {
            return std::vector<uint8_t>();
        }
        std::cerr << "[CLIENT NETWORK] Receive error: " << ec.message() << std::endl;
        return std::vector<uint8_t>();
    }

    if (bytesReceived > 0) {
        buffer.resize(bytesReceived);
        return buffer;
        // TODO(albane) Process buffer and create IPacketManager
        // return your packet manager here
    }
    return std::vector<uint8_t>();
}

std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> UnixClientNetwork::receiveAny() {
    return {};
}

}  //  namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
