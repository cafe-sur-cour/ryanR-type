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
#include <asio.hpp>

#include "../../../common/interfaces/INetworkSocket.hpp"
#include "../../../common/interfaces/IEventLoop.hpp"

namespace net {

class AsioSocket : public INetworkSocket {

    class Impl {
        public:
            std::shared_ptr<asio::ip::udp::socket> socket;
    };

    public:
        explicit AsioSocket(std::shared_ptr<IEventLoop> eventLoop);
        ~AsioSocket() override;
        bool open(std::shared_ptr<INetworkErrorCode> ec) override;
        bool bind(const INetworkEndpoint& endpoint, std::shared_ptr<INetworkErrorCode> ec) override;
        std::size_t sendTo(const std::vector<uint8_t>& data, const INetworkEndpoint& endpoint, int flags, std::shared_ptr<INetworkErrorCode> ec) override;
        std::size_t receiveFrom(std::shared_ptr<std::vector<uint8_t>> buffer, std::shared_ptr<INetworkEndpoint> sender, int flags, std::shared_ptr<INetworkErrorCode> ec) override;
        bool setNonBlocking(bool nonBlocking, std::shared_ptr<INetworkErrorCode> ec) override;
        bool close(std::shared_ptr<INetworkErrorCode> ec) override;
        bool isOpen() const override;

    private:
        std::unique_ptr<Impl> _impl;
};

class AsioSocketFactory {
    public:
        static std::shared_ptr<INetworkSocket> createUdpSocket(std::shared_ptr<IEventLoop> eventLoop);
};

} // namespace net

#endif // NETWORK_SOCKET_HPP
