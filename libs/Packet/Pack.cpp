/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack
*/

#include <vector>
#include <memory>
#include <iostream>
#include <string>
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
        for (uint64_t i = 1; i < payload.size();) {
            for (const auto &[compType, compLength, compSize] : this->_lengthComb) {
                if (payload.at(i) == compType) {
                    if (compType == PROJECTILE_PREFAB) {
                        std::string prefabName;
                        uint64_t j = i + 1;
                        while (j + 2 < payload.size() &&
                               !(payload.at(j)     == static_cast<uint64_t>('\r') &&
                                 payload.at(j + 1) == static_cast<uint64_t>('\n') &&
                                 payload.at(j + 2) == static_cast<uint64_t>('\0'))) {
                            prefabName += static_cast<char>(payload.at(j));
                            j++;
                        }
                        uint32_t nameLength = static_cast<uint32_t>(prefabName.size() + 4);
                        length += nameLength;
                        i = j + 3;
                        break;
                    }
                    length += compLength;
                    i += compSize;
                    break;
                }
            }
        }

        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
        std::vector<uint8_t> body = this->_serializer->serializeULong(payload.at(0));
        packet.insert(packet.end(), body.begin(), body.end());
        for (uint64_t i = 1; i < payload.size();) {
            auto iPtr = std::make_shared<unsigned int>(static_cast<unsigned int>(i));
            for (auto &func : this->_packGSFunction) {
                std::vector<uint8_t> compData = func(payload, iPtr);
                if (!compData.empty()) {
                    packet.insert(packet.end(), compData.begin(), compData.end());
                    i = *iPtr;
                    break;
                }
            }
        }
        return packet;
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
        if (type == CAN_START_PACKET) {
            length = static_cast<uint32_t>(payload.size()) * sizeof(uint64_t);
        } else {
            debug::Debug::printDebug(true, "[PACKET] Warning: Packet size not fixed " +
            std::to_string(static_cast<int>(type)) + " for packing",
            debug::debugType::NETWORK, debug::debugLevel::WARNING);
            length = static_cast<uint32_t>(payload.size()) * sizeof(uint64_t);
        }
    }

    temp = this->_serializer->serializeUInt(length);
    packet.insert(packet.end(), temp.begin(), temp.end());
    for (auto &handler : this->_packetHandlers) {
        if (handler.first == type) {
            std::vector<uint8_t> body = handler.second(payload);
            packet.insert(packet.end(), body.begin(), body.end());
            break;
        }
    }
    return packet;
}
