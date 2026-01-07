/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers
*/

#include "DefaultPacketHandlers.hpp"
#include "../../common/packet/DefaultPacketHandlers.hpp"

namespace rcli::packet {

bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    return common::packet::registerDefaultPacketHandlers(packet);
}

}
