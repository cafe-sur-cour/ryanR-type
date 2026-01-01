/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint - Wrapper pour endpoint réseau (indépendant d'ASIO)
*/

#ifndef NETWORKENDPOINT_HPP_
#define NETWORKENDPOINT_HPP_

#include <string>
#include <cstdint>
#include <asio.hpp>

namespace net {

class NetworkEndpoint {
    public:
        NetworkEndpoint() = default;

        NetworkEndpoint(const std::string& address, uint16_t port)
            : _address(address), _port(port) {}

        explicit NetworkEndpoint(const asio::ip::udp::endpoint& asioEndpoint)
            : _address(asioEndpoint.address().to_string()),
            _port(asioEndpoint.port()) {}

        asio::ip::udp::endpoint toAsioEndpoint() const {
            return asio::ip::udp::endpoint(
                asio::ip::address::from_string(_address),
                _port
            );
        }

        const std::string& getAddress() const { return _address; }
        uint16_t getPort() const { return _port; }

        void setAddress(const std::string& address) { _address = address; }
        void setPort(uint16_t port) { _port = port; }

        bool operator==(const NetworkEndpoint& other) const {
            return _address == other._address && _port == other._port;
        }

        bool operator!=(const NetworkEndpoint& other) const {
            return !(*this == other);
        }

        bool operator<(const NetworkEndpoint& other) const {
            if (_address != other._address)
                return _address < other._address;
            return _port < other._port;
        }

    private:
        std::string _address;
        uint16_t _port = 0;
};

} // namespace net

#endif /* !NETWORKENDPOINT_HPP_ */
