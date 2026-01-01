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

namespace net {
    class NetworkAddress;
}

namespace asio {
    namespace ip {
        class address;
        namespace detail {
            class udp_endpoint;
        }
        template <typename InternetProtocol>
        class basic_endpoint;
        class udp;
        typedef basic_endpoint<udp> udp_endpoint;
    }
}

namespace net {

class NetworkEndpoint {
    public:
        NetworkEndpoint() = default;
        NetworkEndpoint(const std::string& address, uint16_t port)
            : _address(address), _port(port) {}
        NetworkEndpoint(const NetworkAddress& address, uint16_t port);

        explicit NetworkEndpoint(const asio::ip::udp_endpoint& asioEndpoint);

        asio::ip::udp_endpoint toAsioEndpoint() const;

        const std::string& getAddress() const;
        uint16_t getPort() const;

        void setAddress(const std::string& address);
        void setPort(uint16_t port);

        bool operator==(const NetworkEndpoint& other) const;
        bool operator!=(const NetworkEndpoint& other) const;
        bool operator<(const NetworkEndpoint& other) const;

    private:
        std::string _address;
        uint16_t _port = 0;
};

} // namespace net

#endif /* !NETWORKENDPOINT_HPP_ */
