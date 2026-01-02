/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkResolver implementation avec ASIO
*/

#include <asio.hpp>
#include <stdexcept>
#include "Resolver.hpp"
#include "ErrorCode.hpp"
#include "EventLoop.hpp"

class net::AsioResolver::Impl {
public:
    std::shared_ptr<asio::ip::udp::resolver> resolver;
};

net::AsioResolver::AsioResolver(std::shared_ptr<IEventLoop> eventLoop) : _impl(std::make_unique<Impl>()) {
    auto asioLoop = std::dynamic_pointer_cast<AsioEventLoop>(eventLoop);
    if (!asioLoop) {
        throw std::runtime_error("EventLoop is not an AsioEventLoop");
    }
    _impl->resolver = std::make_shared<asio::ip::udp::resolver>(asioLoop->getIoContext());
}

net::AsioResolver::~AsioResolver() = default;

std::vector<net::AsioEndpoint> net::AsioResolver::resolve(const std::string& host,
    const std::string& port, INetworkErrorCode& ec) {
    std::vector<AsioEndpoint> endpoints;
    auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
    try {
        auto results = _impl->resolver->resolve(asio::ip::udp::v4(), host, port, *asioEc);
        if (!ec.hasError()) {
            for (const auto& entry : results) {
                endpoints.emplace_back(AsioEndpoint(entry.endpoint()));
            }
        }
    } catch (const std::exception&) {
        ec.setError(NetworkError::OTHER, "Resolution failed");
    }
    return endpoints;
}

std::shared_ptr<net::INetworkResolver> net::NetworkResolverFactory::createUdpResolver(
    std::shared_ptr<IEventLoop> eventLoop) {
    return std::make_shared<net::AsioResolver>(eventLoop);
}
