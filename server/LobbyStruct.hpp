/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** Lobby
*/

#include <string>
#include <vector>
#include <thread>
#include <asio/ip/udp.hpp>

#ifndef LOBBYSTRUCT_HPP_
#define LOBBYSTRUCT_HPP_

namespace rserv {

class Lobby;

struct LobbyStruct {

    std::string _lobbyCode;
    std::vector<std::tuple<uint8_t, asio::ip::udp::endpoint, std::string>> _clients;
    std::shared_ptr<Lobby> _lobby;
};

}

#endif /* !LOBBYSTRUCT_HPP_ */
