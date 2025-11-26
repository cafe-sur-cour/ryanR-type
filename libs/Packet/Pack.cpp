/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack
*/

#include <vector>
#include "PacketManager.hpp"

std::vector<uint8_t> PacketManager::pack(uint8_t idClient,
    uint32_t sequenceNumber, uint8_t type) {
    std::vector<uint8_t> header;
    std::vector<uint8_t> temp;
    uint32_t length = 0;

    temp = this->_serializer->serializeUChar(this->_magicNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(idClient);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUInt(sequenceNumber);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(type);
    header.insert(header.end(), temp.begin(), temp.end());

    for (auto &pair : this->_packetLengths) {
        if (pair.first == type) {
            length = pair.second;
            break;
        }
    }
    temp = this->_serializer->serializeUInt(length);
    header.insert(header.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUShort(this->_endOfPacket);
    header.insert(header.end(), temp.begin(), temp.end());
    return header;
}

std::vector<uint8_t> PacketManager::pack(std::vector<uint64_t> payload) {
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
