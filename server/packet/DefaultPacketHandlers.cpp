/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers registration (server-side)
*/

// forward to common implementation
#include "../../common/packet/DefaultPacketHandlers.hpp"

namespace rserv::packet {
    bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet) {
        return common::packet::registerDefaultPacketHandlers(packet);
    }
}
