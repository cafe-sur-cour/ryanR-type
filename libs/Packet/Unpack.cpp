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
        for (unsigned int i = 1; i < this->_length;) {
            if (payload.at(i) == TRANSFORM) {
                this->_payload.push_back(static_cast<uint64_t>(TRANSFORM));
                uint64_t posX = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 9));
                uint64_t posY = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 9,
                    payload.begin() + i + 17));
                uint64_t rotation = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 17,
                    payload.begin() + i + 25));
                uint64_t scaleX = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 25,
                    payload.begin() + i + 33));
                uint64_t scaleY = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 33,
                    payload.begin() + i + 41));
                this->_payload.push_back(posX);
                this->_payload.push_back(posY);
                this->_payload.push_back(rotation);
                this->_payload.push_back(scaleX);
                this->_payload.push_back(scaleY);
                i += 41;
                continue;
            } if (payload.at(i) == VELOCITY) {
                this->_payload.push_back(static_cast<uint64_t>(VELOCITY));
                uint64_t velX = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 9));
                uint64_t velY = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 9,
                    payload.begin() + i + 17));
                this->_payload.push_back(velX);
                this->_payload.push_back(velY);
                i += 17;
                continue;
            } else if (payload.at(i) == SPEED) {
                this->_payload.push_back(static_cast<uint64_t>(SPEED));
                uint64_t speed = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 9));
                this->_payload.push_back(speed);
                i += 9;
                continue;
            } else if (payload.at(i) == HEALTH) {
                this->_payload.push_back(static_cast<uint64_t>(HEALTH));
                uint64_t health = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 9));
                uint64_t baseHealth = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 9,
                    payload.begin() + i + 17));
                this->_payload.push_back(health);
                this->_payload.push_back(baseHealth);
                i += 17;
                continue;
            } else if (payload.at(i) == COLLIDER) {
                this->_payload.push_back(static_cast<uint64_t>(COLLIDER));
                uint64_t offsetX = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 9));
                uint64_t offsetY = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 9,
                    payload.begin() + i + 17));
                uint64_t sizeX = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 17,
                    payload.begin() + i + 25));
                uint64_t sizeY = this->_serializer->deserializeULong(
                    std::vector<uint8_t>(payload.begin() + i + 25,
                    payload.begin() + i + 33));
                uint64_t Collisiontype = this->_serializer->deserializeUChar(
                    std::vector<uint8_t>(payload.begin() + i + 33,
                    payload.begin() + i + 34));
                this->_payload.push_back(offsetX);
                this->_payload.push_back(offsetY);
                this->_payload.push_back(sizeX);
                this->_payload.push_back(sizeY);
                this->_payload.push_back(Collisiontype);
                i += 34;
                continue;
            } else if (payload.at(i) == PLAYER_TAG) {
                this->_payload.push_back(static_cast<uint64_t>(PLAYER_TAG));
                uint64_t tag = this->_serializer->deserializeUChar(
                    std::vector<uint8_t>(payload.begin() + i + 1,
                    payload.begin() + i + 2));
                this->_payload.push_back(tag);
                i += 2;
                continue;
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
    debug::Debug::printDebug(true,
        "[PACKET] Unknown packet type "
        + std::to_string(static_cast<int>(type))
        + " for unpacking",
        debug::debugType::NETWORK, debug::debugLevel::ERROR);
    return false;
}
