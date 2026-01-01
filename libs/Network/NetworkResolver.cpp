/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkResolver implementation avec ASIO
*/

#include "NetworkResolver.hpp"
#include "NetworkErrorCode.hpp"
#include "AsioEventLoop.hpp"
#include <asio.hpp>
#include <stdexcept>

namespace net {

class AsioNetworkResolver : public INetworkResolver {
    public:
        explicit AsioNetworkResolver(std::shared_ptr<IEventLoop> eventLoop) {
            auto asioLoop = std::dynamic_pointer_cast<AsioEventLoop>(eventLoop);
            if (!asioLoop) {
                throw std::runtime_error("EventLoop is not an AsioEventLoop");
            }
            _resolver = std::make_shared<asio::ip::udp::resolver>(asioLoop->getIoContext());
        }

        ~AsioNetworkResolver() override = default;

        std::vector<NetworkEndpoint> resolve(const std::string& host,
            const std::string& port, NetworkErrorCode& ec) override {
            std::vector<NetworkEndpoint> endpoints;
            auto* asioEc = static_cast<asio::error_code*>(ec.getInternalErrorCode());
            try {
                auto results = _resolver->resolve(asio::ip::udp::v4(), host, port, *asioEc);
                if (!ec.hasError()) {
                    for (const auto& entry : results) {
                        endpoints.emplace_back(entry.endpoint());
                    }
                }
            } catch (const std::exception&) {
                ec.setError(NetworkError::OTHER, "Resolution failed");
            }
            return endpoints;
        }

    private:
        std::shared_ptr<asio::ip::udp::resolver> _resolver;
};

std::shared_ptr<INetworkResolver> NetworkResolverFactory::createUdpResolver(
    std::shared_ptr<IEventLoop> eventLoop) {
    return std::make_shared<AsioNetworkResolver>(eventLoop);
}

} // namespace net
