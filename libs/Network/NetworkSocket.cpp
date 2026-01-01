/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkSocket implementation avec ASIO
*/

#include "NetworkSocket.hpp"
#include "NetworkEndpoint.hpp"
#include "NetworkErrorCode.hpp"
#include "AsioEventLoop.hpp"
#include <asio.hpp>
#include <stdexcept>

namespace net {

class AsioNetworkSocket : public INetworkSocket {
    public:
        explicit AsioNetworkSocket(std::shared_ptr<IEventLoop> eventLoop) {
            auto asioLoop = std::dynamic_pointer_cast<AsioEventLoop>(eventLoop);
            if (!asioLoop) {
                throw std::runtime_error("EventLoop is not an AsioEventLoop");
            }
            _socket = std::make_shared<asio::ip::udp::socket>(asioLoop->getIoContext());
        }

        ~AsioNetworkSocket() override = default;

        bool open(NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            _socket->open(asio::ip::udp::v4(), *asioEc);
            return !ec.hasError();
        }

        bool bind(const NetworkEndpoint& endpoint, NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            _socket->bind(endpoint.toAsioEndpoint(), *asioEc);
            return !ec.hasError();
        }

        std::size_t sendTo(const std::vector<uint8_t>& data, const NetworkEndpoint& endpoint,
            int flags, NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            return _socket->send_to(asio::buffer(data), endpoint.toAsioEndpoint(), flags, *asioEc);
        }

        std::size_t receiveFrom(std::vector<uint8_t>& buffer, NetworkEndpoint& sender,
            int flags, NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            asio::ip::udp::endpoint asioSender;
            std::size_t bytes = _socket->receive_from(asio::buffer(buffer), asioSender, flags, *asioEc);
            if (!ec.hasError()) {
                sender = NetworkEndpoint(asioSender);
            }

            return bytes;
        }

        bool setNonBlocking(bool nonBlocking, NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            _socket->non_blocking(nonBlocking, *asioEc);
            return !ec.hasError();
        }

        bool close(NetworkErrorCode& ec) override {
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            _socket->close(*asioEc);
            return !ec.hasError();
        }

        bool isOpen() const override {
            return _socket->is_open();
        }

    private:
        std::shared_ptr<asio::ip::udp::socket> _socket;
};

std::shared_ptr<INetworkSocket> NetworkSocketFactory::createUdpSocket(
    std::shared_ptr<IEventLoop> eventLoop) {
    return std::make_shared<AsioNetworkSocket>(eventLoop);
}

} // namespace net
