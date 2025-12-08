/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack
*/

#include <vector>
#include <iostream>
#include "PacketManager.hpp"
#include "../../common/debug.hpp"

std::vector<uint8_t> pm::PacketManager::pack(uint8_t idClient, uint32_t sequenceNumber,
    uint8_t type, std::vector<uint64_t> payload) {
    std::vector<uint8_t> packet;
    std::vector<uint8_t> temp;
    uint32_t length = 0;

    temp = this->_serializer->serializeUChar(MAGIC_NUMBER);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(idClient);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUInt(sequenceNumber);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(type);
    packet.insert(packet.end(), temp.begin(), temp.end());

    for (auto &pair : this->_packetLengths) {
        if (pair.first == type) {
            length = pair.second;
            break;
        }
    }

    if (length == 0) {
        if (type != CAN_START_PACKET) {
            return {};
        }
        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
        return packet;
    } else {
        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
    }

    for (auto &handler : this->_packetHandlers) {
        if (handler.first == type) {
            std::vector<uint8_t> body = handler.second(payload);
            packet.insert(packet.end(), body.begin(), body.end());
            break;
        }
    }
    return packet;
}
