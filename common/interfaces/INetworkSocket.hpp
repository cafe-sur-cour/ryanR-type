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

namespace net {

class INetworkEndpoint;
class INetworkErrorCode;

class INetworkSocket {
    public:
        virtual ~INetworkSocket() = default;
        virtual bool open(std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual bool bind(const INetworkEndpoint& endpoint, std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual std::size_t sendTo(const std::vector<uint8_t>& data, const INetworkEndpoint& endpoint, int flags, std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual std::size_t receiveFrom(std::shared_ptr<std::vector<uint8_t>> buffer, std::shared_ptr<INetworkEndpoint> sender, int flags, std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual bool setNonBlocking(bool nonBlocking, std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual bool close(std::shared_ptr<INetworkErrorCode> ec) = 0;
        virtual bool isOpen() const = 0;
};

} // namespace net

#endif // INETWORK_SOCKET_HPP
