/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Unpack
*/

#include <vector>
#include <iostream>
#include "PacketManager.hpp"

bool PacketManager::unpack(std::vector<uint8_t> data) {
    if (data.empty()) {
        std::cerr <<
            "[PACKET] Error: unpackPacket(): Empty packet data"
            << std::endl;
        return false;
    }

    if (data.at(0) == this->_magicNumber)
        return this->unpackHeader(data);
    return this->unpackBody(data);
}

bool PacketManager::unpackHeader(std::vector<uint8_t> data) {
    if (data.empty() || data.size() != HEADER_SIZE) {
        std::cerr <<
            "[PACKET] Error: unpackPacket(): Invalid header data"
            << std::endl;
        return false;
    }

    this->_idClient = data.at(1);
    this->_sequenceNumber = (static_cast<uint32_t>(data.at(2)) << 24) |
        (static_cast<uint32_t>(data.at(3)) << 16) |
        (static_cast<uint32_t>(data.at(4)) << 8) |
        static_cast<uint32_t>(data.at(5));
    this->_type = data.at(6);
    this->_length = (static_cast<uint32_t>(data.at(7)) << 24) |
        (static_cast<uint32_t>(data.at(8)) << 16) |
        (static_cast<uint32_t>(data.at(9)) << 8) |
        static_cast<uint32_t>(data.at(10));

    if (data.at(11) != (FIRST_EOP_CHAR) ||
        data.at(12) != (SECOND_EOP_CHAR)) {
        std::cerr <<
            "[PACKET] Error: unpackPacket(): Invalid end of packet characters"
            << std::endl;
        return false;
    }
    return true;
}

bool PacketManager::unpackBody(std::vector<uint8_t> data) {
    if (this->_type == data.at(0)) {
        for (auto &received : this->_packetReceived) {
            if (received.first == this->_type) {
                return received.second(data);
            }
        }
        std::cerr <<
            "[PACKET] Error: unpackPacket(): Unknown packet type received"
            << std::endl;
        return false;
    }
    std::cerr <<
        "[PACKET] Error: unpackPacket(): Packet type mismatch"
        << std::endl;
    return false;
}
