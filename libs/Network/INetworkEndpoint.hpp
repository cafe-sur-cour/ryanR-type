/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkEndpoint - Wrapper pour endpoint réseau (indépendant d'ASIO)
*/

#ifndef INETWORKENDPOINT_HPP_
#define INETWORKENDPOINT_HPP_

#include <string>
#include <cstdint>
#include "Asio/Address.hpp"

namespace net {

class INetworkEndpoint {
    public:
        virtual ~INetworkEndpoint() noexcept = default;
        
        virtual const std::string& getAddress() const = 0;
        virtual uint16_t getPort() const = 0;

        virtual void setAddress(const std::string& address) = 0;
        virtual void setPort(uint16_t port) = 0;
        virtual bool operator==(const INetworkEndpoint& other) const = 0;
        virtual bool operator!=(const INetworkEndpoint& other) const = 0;
        virtual bool operator<(const INetworkEndpoint& other) const = 0;
};

} // namespace net

#endif /* !INETWORKENDPOINT_HPP_ */
