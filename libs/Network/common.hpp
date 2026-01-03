/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef COMMON_HPP_
    #define COMMON_HPP_

#include "NetworkConfig.hpp"

using NetworkErrorCode = net::NetworkErrorCode;
using NetworkAddress = net::NetworkAddress;
using NetworkEndpoint = net::NetworkEndpoint;
using NetworkSocket = net::NetworkSocket;
using NetworkResolver = net::NetworkResolver;
using NetworkEventLoop = net::NetworkEventLoop;

constexpr std::size_t MAX_UDP_PACKET_SIZE = 65536;

#endif /* !COMMON_HPP_ */
