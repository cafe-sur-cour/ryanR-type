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

    this->_idClient = static_cast<uint8_t>(this->_serializer->deserializeUChar(
        std::vector<uint8_t>(data.begin() + 1, data.begin() + 2)));
    this->_sequenceNumber = static_cast<uint32_t>(
        this->_serializer->deserializeUInt(
        std::vector<uint8_t>(data.begin() + 2, data.begin() + 6)));
    this->_type = static_cast<uint8_t>(this->_serializer->deserializeUChar(
        std::vector<uint8_t>(data.begin() + 6, data.begin() + 7)));
    this->_length = static_cast<uint32_t>(this->_serializer->deserializeUInt(
        std::vector<uint8_t>(data.begin() + 7, data.begin() + 11)));
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
