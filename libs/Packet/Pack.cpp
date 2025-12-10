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
#include "../../common/translationToECS.hpp"

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

    if (type == GAME_STATE_PACKET) {
        length += 8;
        for (uint64_t i = 1; i < payload.size(); i++) {
            if (payload.at(i) == TRANSFORM) {
                length += 41;
            } else if (payload.at(i) == VELOCITY) {
                length += 17;
            } else if (payload.at(i) == SPEED) {
                length += 9;
            } else if (payload.at(i) == HEALTH) {
                length += 16;
            } else if (payload.at(i) == COLLIDER) {
                length += 33;
            } else if (payload.at(i) == PLAYER_TAG) {
                length += 8;
            }
        }
    }

    if (length == 0) {
        if (type == END_MAP_PACKET) {
            temp = this->_serializer->serializeUInt(length);
            packet.insert(packet.end(), temp.begin(), temp.end());
            return packet;
        }
        if (type != MAP_SEND_PACKET && type != CAN_START_PACKET) {
            std::cerr << "[PACKET] Error: Unknown packet type "
                << static_cast<int>(type) << " for packing" << std::endl;
            return std::vector<uint8_t>();
        }
        debug::Debug::printDebug(true, "[PACKET] Warning: Packet size not fixed " +
            std::to_string(static_cast<int>(type)) + " for packing",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
        length = static_cast<uint32_t>(payload.size()) * sizeof(uint64_t);
    }

    temp = this->_serializer->serializeUInt(length);
    packet.insert(packet.end(), temp.begin(), temp.end());

    if (type == GAME_STATE_PACKET) {
        std::vector<uint8_t> body = {};
        body = this->_serializer->serializeUDouble(static_cast<uint64_t>(payload.size()));
        packet.insert(packet.end(), body.begin(), body.end());
        for (uint64_t i = 1; i < payload.size();) {
            if (payload.at(i) == TRANSFORM) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 1));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 2));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 3));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 4));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 5));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 6;
                continue;
            } if (payload.at(i) == VELOCITY) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 1));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 2));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 3;
                continue;
            } else if (payload.at(i) == SPEED) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 1));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 2;
                continue;
            } else if (payload.at(i) == HEALTH) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 1));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 2));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 3;
                continue;
            } else if (payload.at(i) == COLLIDER) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 1));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 2));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 3));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUDouble(payload.at(i + 4));
                packet.insert(packet.end(), body.begin(), body.end());
                body = this->_serializer->serializeUChar(payload.at(i + 5));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 6;
                continue;
            } else if (payload.at(i) == PLAYER_TAG) {
                body = this->_serializer->serializeUChar(payload.at(i));
                packet.insert(packet.end(), body.begin(), body.end());
                i += 1;
                continue;
            }
        }
        return packet;
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
