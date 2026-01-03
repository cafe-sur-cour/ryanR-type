/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Network Configuration - Centralized type aliases and implementation selection
*/

#ifndef NETWORK_CONFIG_HPP_
    #define NETWORK_CONFIG_HPP_

#ifndef NETWORK_USE_ASIO
    #define NETWORK_USE_ASIO
#endif

#ifdef NETWORK_USE_ASIO
    #include "Asio/AsioErrorCode.hpp"
    #include "Asio/AsioAddress.hpp"
    #include "Asio/AsioEndpoint.hpp"
    #include "Asio/AsioSocket.hpp"
    #include "Asio/AsioResolver.hpp"
    #include "Asio/AsioEventLoop.hpp"

    namespace net {
        using NetworkErrorCode = AsioErrorCode;
        using NetworkAddress = AsioAddress;
        using NetworkEndpoint = AsioEndpoint;
        using NetworkSocket = AsioSocket;
        using NetworkResolver = AsioResolver;
        using NetworkEventLoop = AsioEventLoop;
    } // namespace net
#else
    #error "No network implementation selected. Define NETWORK_USE_ASIO or another implementation."
#endif

#endif /* !NETWORK_CONFIG_HPP_ */
