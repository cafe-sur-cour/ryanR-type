/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint
*/

#ifndef ASIO_ENDPOINT_HPP_
#define ASIO_ENDPOINT_HPP_

#include <string>
#include <cstdint>
#include "AsioAddress.hpp"
#include "../INetworkEndpoint.hpp"

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

class AsioEndpoint : public INetworkEndpoint {
    public:
        AsioEndpoint() = default;
        AsioEndpoint(const std::string& address, uint16_t port)
            : _address(address), _port(port) {}
        AsioEndpoint(const AsioAddress& address, uint16_t port);

        explicit AsioEndpoint(const asio::ip::udp_endpoint& asioEndpoint);

        asio::ip::udp_endpoint toAsioEndpoint() const;

        const std::string& getAddress() const override;
        uint16_t getPort() const override;

        void setAddress(const std::string& address) override;
        void setPort(uint16_t port) override;
        bool operator==(const INetworkEndpoint& other) const override;
        bool operator!=(const INetworkEndpoint& other) const override;
        bool operator<(const INetworkEndpoint& other) const override;

    private:
        std::string _address;
        uint16_t _port = 0;
};

} // namespace net

#endif /* !ASIO_ENDPOINT_HPP_ */
