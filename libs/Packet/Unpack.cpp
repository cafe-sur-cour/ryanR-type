/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Unpack
*/

#include <vector>
#include <iostream>
#include "PacketManager.hpp"
#include "../../common/debug.hpp"

bool pm::PacketManager::unpack(std::vector<uint8_t> data) {
    if (data.empty()) {
        debug::Debug::printDebug(true,
            "[PACKET] Received empty data for unpacking",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    if (data.at(0) != MAGIC_NUMBER) {
        debug::Debug::printDebug(true,
            "[PACKET] Invalid magic number in received data",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }

    uint64_t idClient = 0;
    uint64_t sequenceNumber = 0;
    uint64_t type = 0;
    uint64_t length = 0;
    idClient = this->_serializer->deserializeUChar(
        std::vector<uint8_t>(data.begin() + 1, data.begin() + 2));
    sequenceNumber = this->_serializer->deserializeUInt(
        std::vector<uint8_t>(data.begin() + 2, data.begin() + 6));
    type = this->_serializer->deserializeUChar(
        std::vector<uint8_t>(data.begin() + 6, data.begin() + 7));
    length = this->_serializer->deserializeUInt(
        std::vector<uint8_t>(data.begin() + 7, data.begin() + 11));

    if (data.size() - HEADER_SIZE != length) {
        debug::Debug::printDebug(true,
            "[PACKET] Mismatch between declared length and actual data size",
            debug::debugType::NETWORK, debug::debugLevel::ERROR);
        return false;
    }
    if (length == 0)
        return true;

    for (auto &handler : this->_packetReceived) {
        if (handler.first == type) {
            std::vector<uint8_t> payload(data.begin() + 11, data.end());
            bool result = handler.second(payload);
            if (!result) {
                return false;
            }
            this->_idClient = static_cast<uint8_t>(idClient);
            this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
            this->_type = static_cast<uint8_t>(type);
            this->_length = static_cast<uint32_t>(length);
            return true;
        }
    }

    debug::Debug::printDebug(true,
        "[PACKET] Unknown packet type "
        + std::to_string(static_cast<int>(type))
        + " for unpacking",
        debug::debugType::NETWORK, debug::debugLevel::ERROR);
    return false;
}
