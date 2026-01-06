/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers
*/

#ifndef COMMON_DEFAULT_PACKET_HANDLERS_HPP_
#define COMMON_DEFAULT_PACKET_HANDLERS_HPP_

#include <memory>
#include "../interfaces/IPacketManager.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"

namespace common::packet {

using SerializerPtr = std::shared_ptr<pm::ISerializer>;

inline SerializerPtr makeSerializer() {
    return std::make_shared<pm::BigEndianSerialization>();
}

bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet);

}

#endif // COMMON_DEFAULT_PACKET_HANDLERS_HPP_
