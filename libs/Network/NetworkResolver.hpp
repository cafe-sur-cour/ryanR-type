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
#include "NetworkEndpoint.hpp"

namespace net {

class IEventLoop;
class NetworkErrorCode;

class INetworkResolver {
    public:
        virtual ~INetworkResolver() = default;
        virtual std::vector<NetworkEndpoint> resolve(const std::string& host,
            const std::string& port, NetworkErrorCode& ec) = 0;
};

class NetworkResolverFactory {
    public:
        static std::shared_ptr<INetworkResolver> createUdpResolver(std::shared_ptr<IEventLoop> eventLoop);
};

} // namespace net

#endif // NETWORK_RESOLVER_HPP
