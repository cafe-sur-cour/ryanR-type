/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkSocket
*/

#ifndef NETWORK_SOCKET_HPP
#define NETWORK_SOCKET_HPP

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

#include "../INetworkSocket.hpp"
#include "../IEventLoop.hpp"

namespace net {

class AsioSocket : public INetworkSocket {
    public:
        explicit AsioSocket(std::shared_ptr<IEventLoop> eventLoop);
        ~AsioSocket() override;
        bool open(INetworkErrorCode& ec) override;
        bool bind(const INetworkEndpoint& endpoint, INetworkErrorCode& ec) override;
        std::size_t sendTo(const std::vector<uint8_t>& data, const INetworkEndpoint& endpoint, int flags, INetworkErrorCode& ec) override;
        std::size_t receiveFrom(std::vector<uint8_t>& buffer, INetworkEndpoint& sender, int flags, INetworkErrorCode& ec) override;
        bool setNonBlocking(bool nonBlocking, INetworkErrorCode& ec) override;
        bool close(INetworkErrorCode& ec) override;
        bool isOpen() const override;

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
};

class AsioSocketFactory {
    public:
        static std::shared_ptr<INetworkSocket> createUdpSocket(std::shared_ptr<IEventLoop> eventLoop);
};

} // namespace net

#endif // NETWORK_SOCKET_HPP
