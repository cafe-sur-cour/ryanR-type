/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkSocket implementation avec ASIO
*/

#include <asio.hpp>
#include <stdexcept>
#include "Socket.hpp"
#include "Endpoint.hpp"
#include "ErrorCode.hpp"
#include "EventLoop.hpp"

class net::AsioSocket::Impl {
public:
    std::shared_ptr<asio::ip::udp::socket> socket;
};

net::AsioSocket::AsioSocket(std::shared_ptr<IEventLoop> eventLoop) : _impl(std::make_unique<Impl>()) {
    auto asioLoop = std::dynamic_pointer_cast<AsioEventLoop>(eventLoop);
    if (!asioLoop) {
        throw std::runtime_error("EventLoop is not an AsioEventLoop");
    }
    _impl->socket = std::make_shared<asio::ip::udp::socket>(asioLoop->getIoContext());
}

net::AsioSocket::~AsioSocket() = default;

bool net::AsioSocket::open(INetworkErrorCode& ec) {
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    _impl->socket->open(asio::ip::udp::v4(), *asioEc);
    return !ec.hasError();
}

bool net::AsioSocket::bind(const INetworkEndpoint& endpoint, INetworkErrorCode& ec) {
    auto asioEndpoint = static_cast<const AsioEndpoint&>(endpoint);
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    _impl->socket->bind(asioEndpoint.toAsioEndpoint(), *asioEc);
    return !ec.hasError();
}

std::size_t net::AsioSocket::sendTo(const std::vector<uint8_t>& data, const INetworkEndpoint& endpoint,
    int flags, INetworkErrorCode& ec) {
    auto asioEndpoint = static_cast<const AsioEndpoint&>(endpoint);
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    return _impl->socket->send_to(asio::buffer(data), asioEndpoint.toAsioEndpoint(), flags, *asioEc);
}

std::size_t net::AsioSocket::receiveFrom(std::vector<uint8_t>& buffer, INetworkEndpoint& sender,
    int flags, INetworkErrorCode& ec) {
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    asio::ip::udp::endpoint asioSender;
    std::size_t bytes = _impl->socket->receive_from(asio::buffer(buffer), asioSender, flags, *asioEc);
    if (!ec.hasError()) {
        sender.setAddress(asioSender.address().to_string());
        sender.setPort(asioSender.port());
    }

    return bytes;
}

bool net::AsioSocket::setNonBlocking(bool nonBlocking, INetworkErrorCode& ec) {
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    _impl->socket->non_blocking(nonBlocking, *asioEc);
    return !ec.hasError();
}

bool net::AsioSocket::close(INetworkErrorCode& ec) {
    auto asioEc = std::static_pointer_cast<asio::error_code>(ec.getInternalErrorCode());
    _impl->socket->close(*asioEc);
    return !ec.hasError();
}

bool net::AsioSocket::isOpen() const {
    return _impl->socket->is_open();
}

std::shared_ptr<net::INetworkSocket> net::AsioSocketFactory::createUdpSocket(
    std::shared_ptr<net::IEventLoop> eventLoop) {
    return std::make_shared<AsioSocket>(eventLoop);
}
