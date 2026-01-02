/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkResolver - Abstraction pour la résolution DNS
*/

#ifndef NETWORK_RESOLVER_HPP
#define NETWORK_RESOLVER_HPP

#include <memory>
#include <string>
#include <vector>

#include "Endpoint.hpp"
#include "../INetworkResolver.hpp"
#include "../IEventLoop.hpp"

namespace net {

class AsioResolver : public INetworkResolver {
    public:
        AsioResolver(std::shared_ptr<IEventLoop> eventLoop);
        ~AsioResolver() override;
        std::vector<AsioEndpoint> resolve(const std::string& host, const std::string& port, INetworkErrorCode& ec) override;

    private:
        class Impl;
        std::unique_ptr<Impl> _impl;
};

class NetworkResolverFactory {
    public:
        static std::shared_ptr<INetworkResolver> createUdpResolver(std::shared_ptr<IEventLoop> eventLoop);
};

} // namespace net

#endif // NETWORK_RESOLVER_HPP
