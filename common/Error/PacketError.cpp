/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Packet Error
*/

#include "PacketError.hpp"

err::PacketError::PacketError(const std::string &message, ErrorCode code) :
    AError(message, code) {
}

err::PacketError::~PacketError() {
}

std::string err::PacketError::getType() const noexcept {
    return "PacketError";
}