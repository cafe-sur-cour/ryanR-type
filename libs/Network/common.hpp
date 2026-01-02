/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef COMMON_HPP_
    #define COMMON_HPP_

#include "Asio/AsioErrorCode.hpp"
#include "Asio/AsioAddress.hpp"
#include "Asio/AsioEndpoint.hpp"
#include "Asio/AsioSocket.hpp"
#include "Asio/AsioResolver.hpp"
#include "Asio/AsioEventLoop.hpp"

using NetworkErrorCode = net::AsioErrorCode;
using NetworkAddress = net::AsioAddress;
using NetworkEndpoint = net::AsioEndpoint;
using NetworkSocket = net::AsioSocket;
using NetworkResolver = net::AsioResolver;
using NetworkEventLoop = net::AsioEventLoop;

#endif /* !COMMON_HPP_ */
