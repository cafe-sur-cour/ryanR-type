/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers registration (common)
*/

#ifndef COMMON_DEFAULT_PACKET_HANDLERS_HPP_
#define COMMON_DEFAULT_PACKET_HANDLERS_HPP_

#include <memory>
#include "../../libs/Packet/IPacketManager.hpp"

namespace common::packet {
    bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet);
}

#endif // COMMON_DEFAULT_PACKET_HANDLERS_HPP_
