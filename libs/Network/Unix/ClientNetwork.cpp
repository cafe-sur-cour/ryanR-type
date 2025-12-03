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

// void UnixClientNetwork::init(uint32_t port, const std::string host) {
//     if (!_socket) {
//         _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
//     }

//     if (!_socket->is_open()) {
//         _socket->open(asio::ip::udp::v4());
//     }
//     std::cout << "Socket is open" << std::endl;
//     try {
//         asio::ip::udp::resolver resolver(*_ioContext);
//         asio::ip::udp::resolver::results_type endpoints =
//             resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));

//         _serverEndpoint = *endpoints.begin();
//         this->setConnectionState(ConnectionState::CONNECTING);
//         // this->_socket->non_blocking(true);
//     } catch (const std::exception& e) {
//         _connected = false;
//         throw std::runtime_error(std::string
//             ("[UnixClientNetwork] Connection failed: ") + e.what());
//     }
// 
void UnixClientNetwork::init(uint16_t port, const std::string host) {
    std::error_code ec;
    
    if (!_socket) {
        _socket = std::make_shared<asio::ip::udp::socket>(*_ioContext);
    }
    
    if (!_socket->is_open()) {
        _socket->open(asio::ip::udp::v4(), ec);
        if (ec) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to open socket: ") + ec.message());
        }
    }
    
    std::cout << "[CLIENT NETWORK] Socket is open" << std::endl;
    
    try {
        asio::ip::udp::resolver resolver(*_ioContext);
        asio::ip::udp::resolver::results_type endpoints =
            resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));
        
        if (endpoints.empty()) {
            throw std::runtime_error(
                "[CLIENT NETWORK] Failed to resolve host '" + host + "': no results");
        }
        std::cout << "[CLIENT NETWORK] Resolved host " << host << std::endl;
        _serverEndpoint = *endpoints.begin();
        
        // Set socket to non-blocking mode
        _socket->non_blocking(true, ec);
        if (ec) {
            throw std::runtime_error(
                std::string("[CLIENT NETWORK] Failed to set non-blocking mode: ") + ec.message());
        }
        
        this->setConnectionState(ConnectionState::CONNECTING);
        
        std::cout << "[CLIENT NETWORK] Configured for server at " 
                  << _serverEndpoint.address().to_string() << ":" 
                  << _serverEndpoint.port() << std::endl;
        
    } catch (const std::exception& e) {
        _connected = false;
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
}

bool UnixClientNetwork::isConnected() const {
    return _connected;
}

uint8_t UnixClientNetwork::acceptConnection(asio::ip::udp::endpoint id, std::shared_ptr<pm::IPacketManager> packetManager) {
    // Not applicable for client
    (void)id;
    (void)packetManager;
    return 1;
}

void UnixClientNetwork::sendTo(asio::ip::udp::endpoint id, std::vector<uint8_t> packet) {
   (void)id;  // In client, id is not used
   std::cout << "Sending to " << _serverEndpoint.address().to_string() << ":"
             << _serverEndpoint.port() << " Data size: " << packet.size() << std::endl;
    _socket->send_to(asio::buffer(packet), _serverEndpoint);
}

void UnixClientNetwork::broadcast(const pm::IPacketManager &packet) {
    // For client, broadcast is the same as sending to the server
    // sendTo(0, packet);
    (void)packet;
}

bool UnixClientNetwork::hasIncomingData() const {
    if (!_socket || !_socket->is_open()) {
        return false;
    }

    std::error_code ec;
    size_t available = _socket->available(ec);
    return !ec && available > 0;
}

// std::shared_ptr<pm::IPacketManager> UnixClientNetwork::receiveFrom(
//     const int &connectionId) {
//     std::cout << "Waiting to receive data from server..." << std::endl;
//     (void)connectionId;
    // asio::error_code ec;
    // std::size_t available = _socket->available(ec);
    // std::vector<uint8_t> buffer(available);
    // this->_socket->receive_from(asio::buffer(buffer), _serverEndpoint);
    // std::cout << "Received data from "
    //           << _serverEndpoint.address().to_string() << ":"
    //           << _serverEndpoint.port() << std::endl;
//     return nullptr;
// }

std::shared_ptr<pm::IPacketManager> UnixClientNetwork::receiveFrom(
    const uint8_t &connectionId) {
    (void)connectionId;
    
    asio::error_code ec;
    std::size_t available = _socket->available(ec);
    
    // Check if available() itself failed
    if (ec) {
        std::cerr << "[CLIENT NETWORK] Error checking available data: " 
                  << ec.message() << std::endl;
        return nullptr;
    }
    
    // No data available yet
    if (available == 0) {
        return nullptr;
    }
    
    std::cout << "[CLIENT NETWORK] " << available << " bytes available" << std::endl;
    
    std::vector<uint8_t> buffer(available);
    asio::ip::udp::endpoint senderEndpoint;
    
    std::size_t bytesReceived = _socket->receive_from(
        asio::buffer(buffer), senderEndpoint, 0, ec);
    
    // Handle receive errors
    if (ec) {
        if (ec == asio::error::would_block || ec == asio::error::try_again) {
            // No data available right now (race condition between available() and receive_from())
            return nullptr;
        }
        std::cerr << "[CLIENT NETWORK] Receive error: " << ec.message() << std::endl;
        return nullptr;
    }
    
    if (bytesReceived > 0) {
        std::cout << "[CLIENT NETWORK] Received " << bytesReceived 
                  << " bytes from " << senderEndpoint.address().to_string() 
                  << ":" << senderEndpoint.port() << std::endl;
        
        // Resize buffer to actual bytes received
        buffer.resize(bytesReceived);
        
        // TODO: Process buffer and create IPacketManager
        // return your packet manager here
    }
    
    return nullptr;
}

std::pair<asio::ip::udp::endpoint, std::vector<uint8_t>> UnixClientNetwork::receiveAny() {
    return {};
}

} // namespace net

extern "C" {
    void *createNetworkInstance() {
        return new net::UnixClientNetwork();
    }
    int getType() {
        return NETWORK_CLIENT_MODULE;
    }
}
