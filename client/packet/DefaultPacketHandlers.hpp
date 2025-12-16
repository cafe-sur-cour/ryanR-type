/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers registration (client-side)
*/

#ifndef CLIENT_DEFAULT_PACKET_HANDLERS_HPP_
#define CLIENT_DEFAULT_PACKET_HANDLERS_HPP_

#include <memory>
#include "../../libs/Packet/IPacketManager.hpp"

namespace rcli::packet {
    bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet);
}

#endif // CLIENT_DEFAULT_PACKET_HANDLERS_HPP_
