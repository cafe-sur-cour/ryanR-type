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
#include "../../common/translationToECS.hpp"

bool pm::PacketManager::unpack(std::vector<uint8_t> data) {
    if (data.empty()) {
        return false;
    }

    if (data.at(0) != MAGIC_NUMBER) {
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
        return false;
    }
    if (length == 0)
        return true;

    if (type == GAME_STATE_PACKET) {
        this->_idClient = static_cast<uint8_t>(idClient);
        this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
        this->_type = static_cast<uint8_t>(type);
        this->_length = static_cast<uint32_t>(length);

        std::vector<uint8_t> payload(data.begin() + 11, data.end());
        this->_payload.clear();
        uint64_t idx = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin(), payload.begin() + 8));
        this->_payload.push_back(idx);
        for (unsigned int i = 8; i < payload.size();) {
            std::cout << "Unpacking at index: " << i << " aka: " << static_cast<int>(payload.at(i)) << std::endl;
            for (const auto &func : this->_unpackGSFunction) {
                unsigned int ret = func(payload, i);
                if (ret > 0) {
                    i += ret;
                    break;
                }
            }
        }
        return true;
    }

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
    return false;
}
