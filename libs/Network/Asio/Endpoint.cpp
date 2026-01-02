/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint implementation
*/

#include "Endpoint.hpp"
#include "Address.hpp"
#include <asio.hpp>

net::AsioEndpoint::AsioEndpoint(const net::AsioAddress& address, uint16_t port)
    : _address(address.toString()), _port(port) {
}

net::AsioEndpoint::AsioEndpoint(const asio::ip::udp_endpoint& asioEndpoint)
    : _address(asioEndpoint.address().to_string()),
      _port(asioEndpoint.port()) {
}

asio::ip::udp_endpoint net::AsioEndpoint::toAsioEndpoint() const {
    // Handle special "any" addresses
    if (_address.empty() || _address == "0.0.0.0") {
        return asio::ip::udp_endpoint(asio::ip::udp::v4(), _port);
    }
    if (_address == "::") {
        return asio::ip::udp_endpoint(asio::ip::udp::v6(), _port);
    }
    
    try {
        return asio::ip::udp_endpoint(
            asio::ip::make_address(_address),
            _port
        );
    } catch (const std::exception& e) {
        // Fallback to IPv4 any if address is invalid
        return asio::ip::udp_endpoint(asio::ip::udp::v4(), _port);
    }
}

const std::string& net::AsioEndpoint::getAddress() const {
    return _address;
}

uint16_t net::AsioEndpoint::getPort() const {
    return _port;
}

void net::AsioEndpoint::setAddress(const std::string& address) {
    _address = address;
}

void net::AsioEndpoint::setPort(uint16_t port) {
    _port = port;
}

bool net::AsioEndpoint::operator==(const INetworkEndpoint& other) const {
    return _address == other.getAddress() && _port == other.getPort();
}

bool net::AsioEndpoint::operator!=(const INetworkEndpoint& other) const {
    return !(*this == other);
}

bool net::AsioEndpoint::operator<(const INetworkEndpoint& other) const {
    if (_address != other.getAddress())
        return _address < other.getAddress();
    return _port < other.getPort();
}
