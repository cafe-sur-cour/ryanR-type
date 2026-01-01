/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint implementation
*/

#include "NetworkEndpoint.hpp"
#include "NetworkAddress.hpp"
#include <asio.hpp>

namespace net {

NetworkEndpoint::NetworkEndpoint(const NetworkAddress& address, uint16_t port)
    : _address(address.toString()), _port(port) {
}

NetworkEndpoint::NetworkEndpoint(const asio::ip::udp_endpoint& asioEndpoint)
    : _address(asioEndpoint.address().to_string()),
      _port(asioEndpoint.port()) {
}

asio::ip::udp_endpoint NetworkEndpoint::toAsioEndpoint() const {
    return asio::ip::udp_endpoint(
        asio::ip::make_address(_address),
        _port
    );
}

const std::string& NetworkEndpoint::getAddress() const {
    return _address;
}

uint16_t NetworkEndpoint::getPort() const {
    return _port;
}

void NetworkEndpoint::setAddress(const std::string& address) {
    _address = address;
}

void NetworkEndpoint::setPort(uint16_t port) {
    _port = port;
}

bool NetworkEndpoint::operator==(const NetworkEndpoint& other) const {
    return _address == other._address && _port == other._port;
}

bool NetworkEndpoint::operator!=(const NetworkEndpoint& other) const {
    return !(*this == other);
}

bool NetworkEndpoint::operator<(const NetworkEndpoint& other) const {
    if (_address != other._address)
        return _address < other._address;
    return _port < other._port;
}

} // namespace net
