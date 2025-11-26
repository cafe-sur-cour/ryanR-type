/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack
*/

#include <vector>
#include "Packet.hpp"

std::vector<uint8_t> Packet::pack(uint8_t idClient,
    uint32_t sequenceNumber, uint8_t type) {
    std::vector<uint8_t> header;
    std::vector<uint8_t> temp;
    uint32_t length = 0;

    temp = this->_serializer->serializeChar(this->_magicNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeChar(idClient);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeInt(sequenceNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeChar(type);
    header.insert(header.end(), temp.begin(), temp.end());

    for (auto &pair : this->_packetLengths) {
        if (pair.first == type) {
            length = pair.second;
            break;
        }
    }
    temp = this->_serializer->serializeInt(length);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeShort(this->_endOfPacket);
    header.insert(header.end(), temp.begin(), temp.end());
    return header;
}

std::vector<uint8_t> Packet::pack(std::vector<uint64_t> payload) {
    std::vector<uint8_t> body;
    uint64_t type = NO_OP_PACKET;

    if (payload.empty()) {
        return body;
    }

    type = payload.at(0);
    for (auto &handler : this->_packetHandlers) {
        if (handler.first == type) {
            body = handler.second(payload);
            break;
        }
    }
    return body;
}
