/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef INETWORK_SOCKET_HPP
    #define INETWORK_SOCKET_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "Asio/ErrorCode.hpp"
#include "Asio/Endpoint.hpp"

namespace net {

class INetworkSocket {
    public:
        virtual ~INetworkSocket() = default;
        virtual bool open(INetworkErrorCode& ec) = 0;
        virtual bool bind(const INetworkEndpoint& endpoint, INetworkErrorCode& ec) = 0;
        virtual std::size_t sendTo(const std::vector<uint8_t>& data, const INetworkEndpoint& endpoint, int flags, INetworkErrorCode& ec) = 0;
        virtual std::size_t receiveFrom(std::vector<uint8_t>& buffer, INetworkEndpoint& sender, int flags, INetworkErrorCode& ec) = 0;
        virtual bool setNonBlocking(bool nonBlocking, INetworkErrorCode& ec) = 0;
        virtual bool close(INetworkErrorCode& ec) = 0;
        virtual bool isOpen() const = 0;
};

} // namespace net

#endif // INETWORK_SOCKET_HPP
