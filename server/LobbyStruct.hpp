/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** Lobby
*/

#include <string>
#include <vector>
#include <thread>
#include "../common/interfaces/INetwork.hpp"
#include <asio/ip/udp.hpp>

#ifndef LOBBYSTRUCT_HPP_
#define LOBBYSTRUCT_HPP_

namespace rserv {

class Lobby;

struct LobbyStruct {

    std::string _lobbyCode;
    std::vector<std::tuple<uint8_t, std::shared_ptr<net::INetworkEndpoint>, std::string>> _clients;
    std::shared_ptr<Lobby> _lobby;
};

}

#endif /* !LOBBYSTRUCT_HPP_ */
