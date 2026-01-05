/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** INetworkFactory - Factory interface for creating network objects
*/

#ifndef INETWORK_FACTORY_HPP_
    #define INETWORK_FACTORY_HPP_

#include <memory>
#include "INetworkSocket.hpp"
#include "INetworkResolver.hpp"
#include "IEventLoop.hpp"

namespace net {

class INetworkFactory {
    public:
        virtual ~INetworkFactory() = default;
        virtual std::shared_ptr<IEventLoop> createEventLoop() = 0;
        virtual std::shared_ptr<INetworkSocket> createSocket(std::shared_ptr<IEventLoop> eventLoop) = 0;
        virtual std::shared_ptr<INetworkResolver> createResolver(std::shared_ptr<IEventLoop> eventLoop) = 0;
};

} // namespace net

#endif /* !INETWORK_FACTORY_HPP_ */
