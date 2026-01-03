/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** AsioNetworkFactory - ASIO implementation of network factory
*/

#ifndef ASIO_NETWORK_FACTORY_HPP_
    #define ASIO_NETWORK_FACTORY_HPP_

#include "../INetworkFactory.hpp"
#include "AsioSocket.hpp"
#include "AsioResolver.hpp"
#include "AsioEventLoop.hpp"

namespace net {

class AsioNetworkFactory : public INetworkFactory {
    public:
        std::shared_ptr<IEventLoop> createEventLoop() override {
            return std::make_shared<AsioEventLoop>();
        }

        std::shared_ptr<INetworkSocket> createSocket(
            std::shared_ptr<IEventLoop> eventLoop) override {
            return std::make_shared<AsioSocket>(eventLoop);
        }

        std::shared_ptr<INetworkResolver> createResolver(
            std::shared_ptr<IEventLoop> eventLoop) override {
            return std::make_shared<AsioResolver>(eventLoop);
        }
};

} // namespace net

#endif /* !ASIO_NETWORK_FACTORY_HPP_ */
