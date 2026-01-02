/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** NetworkFactory - Main factory accessor
*/

#ifndef NETWORK_FACTORY_HPP_
    #define NETWORK_FACTORY_HPP_

#include "INetworkFactory.hpp"
#include "NetworkConfig.hpp"

#ifdef NETWORK_USE_ASIO
    #include "Asio/AsioNetworkFactory.hpp"
#endif

namespace net {

class NetworkFactory {
    public:
        static std::shared_ptr<INetworkFactory> getDefault() {
    #ifdef NETWORK_USE_ASIO
            return std::make_shared<AsioNetworkFactory>();
    #else
            #error "No network implementation available"
    #endif
        }

        static std::shared_ptr<INetworkFactory>& getInstance() {
            static std::shared_ptr<INetworkFactory> instance = getDefault();
            return instance;
        }

    private:
        NetworkFactory() = delete;
        ~NetworkFactory() = delete;
        NetworkFactory(const NetworkFactory&) = delete;
        NetworkFactory& operator=(const NetworkFactory&) = delete;
};

} // namespace net

#endif /* !NETWORK_FACTORY_HPP_ */
