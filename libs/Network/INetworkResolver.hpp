/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkResolver - Abstraction pour la résolution DNS
*/

#ifndef INETWORK_RESOLVER_HPP
#define INETWORK_RESOLVER_HPP

#include <memory>
#include <string>
#include <vector>

#include "Asio/Endpoint.hpp"
#include "Asio/ErrorCode.hpp"

namespace net {

class INetworkResolver {
    public:
        virtual ~INetworkResolver() = default;
        virtual std::vector<AsioEndpoint> resolve(const std::string& host,
            const std::string& port, INetworkErrorCode& ec) = 0;
};

} // namespace net

#endif // INETWORK_RESOLVER_HPP
