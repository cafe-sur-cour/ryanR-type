/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint implementation
*/

#include "NetworkEndpoint.hpp"
#include <asio.hpp>

namespace net {

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

} // namespace net
