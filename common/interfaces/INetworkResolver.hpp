/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** NetworkResolver
*/

#ifndef INETWORK_RESOLVER_HPP
#define INETWORK_RESOLVER_HPP

#include <memory>
#include <string>
#include <vector>

namespace net {

class INetworkEndpoint;
class INetworkErrorCode;

class INetworkResolver {
    public:
        virtual ~INetworkResolver() = default;
        virtual std::vector<std::shared_ptr<INetworkEndpoint>> resolve(const std::string& host,
            const std::string& port, std::shared_ptr<INetworkErrorCode> ec) = 0;
};

} // namespace net

#endif // INETWORK_RESOLVER_HPP
