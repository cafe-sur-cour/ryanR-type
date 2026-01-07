/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers
*/

#ifndef DEFAULT_PACKET_HANDLERS_HPP_
#define DEFAULT_PACKET_HANDLERS_HPP_

#include <memory>
#include "../../common/interfaces/IPacketManager.hpp"

namespace rserv::packet {
    bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet);
}

#endif // DEFAULT_PACKET_HANDLERS_HPP_
