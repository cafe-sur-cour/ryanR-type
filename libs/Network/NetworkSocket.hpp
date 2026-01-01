/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkSocket - Abstraction pour les sockets UDP
*/

#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

namespace net {

class NetworkEndpoint;
class IEventLoop;
class NetworkErrorCode;

class INetworkSocket {
public:
    virtual ~INetworkSocket() = default;
    virtual bool open(NetworkErrorCode& ec) = 0;
    virtual bool bind(const NetworkEndpoint& endpoint, NetworkErrorCode& ec) = 0;
    virtual std::size_t sendTo(const std::vector<uint8_t>& data, const NetworkEndpoint& endpoint, int flags, NetworkErrorCode& ec) = 0;
    virtual std::size_t receiveFrom(std::vector<uint8_t>& buffer, NetworkEndpoint& sender, int flags, NetworkErrorCode& ec) = 0;
    virtual bool setNonBlocking(bool nonBlocking, NetworkErrorCode& ec) = 0;
    virtual bool close(NetworkErrorCode& ec) = 0;
    virtual bool isOpen() const = 0;
};

class NetworkSocketFactory {
    public:
        static std::shared_ptr<INetworkSocket> createUdpSocket(std::shared_ptr<IEventLoop> eventLoop);
};

} // namespace net

#endif // NETWORK_SOCKET_HPP
