/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Unpack
*/

#include <vector>
#include <iostream>
#include "PacketManager.hpp"
#include "compression/Compression.hpp"
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
    if (length == 0) {
        this->_idClient = static_cast<uint8_t>(idClient);
        this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
        this->_type = static_cast<uint8_t>(type);
        this->_length = static_cast<uint32_t>(length);
        return true;
    }

    if (type == GAME_STATE_COMPRESSED_PACKET) {
        this->_idClient = static_cast<uint8_t>(idClient);
        this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
        this->_type = GAME_STATE_PACKET;
        std::vector<uint8_t> compressedPayload(data.begin() + 11, data.end());
        std::vector<uint8_t> payload = Compression::decompress(compressedPayload);
        if (payload.empty()) {
            std::cerr << "[PACKET] Failed to decompress GAME_STATE packet" << std::endl;
            return false;
        }

        this->_length = static_cast<uint32_t>(payload.size());
        this->_payload.clear();

        auto [idx, bytesRead] = this->_serializer->deserializeVarint(payload, 0);
        this->_payload.push_back(idx);

        for (unsigned int i = static_cast<unsigned int>(bytesRead); i < payload.size();) {
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

    if (type == GAME_STATE_BATCH_PACKET || type == GAME_STATE_BATCH_COMPRESSED_PACKET) {
        this->_idClient = static_cast<uint8_t>(idClient);
        this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
        this->_type = GAME_STATE_BATCH_PACKET;

        std::vector<uint8_t> payload;
        if (type == GAME_STATE_BATCH_COMPRESSED_PACKET) {
            std::vector<uint8_t> compressedPayload(data.begin() + 11, data.end());
            payload = Compression::decompress(compressedPayload);
            if (payload.empty()) {
                std::cerr << "[PACKET] Failed to decompress GAME_STATE_BATCH packet"
                    << std::endl;
                return false;
            }
        } else {
            payload = std::vector<uint8_t>(data.begin() + 11, data.end());
        }

        this->_length = static_cast<uint32_t>(payload.size());
        this->_batchedPayloads.clear();

        size_t offset = 0;
        auto [entityCount, countBytes] = this->_serializer->deserializeVarint(payload, offset);
        offset += countBytes;

        for (uint64_t e = 0; e < entityCount && offset < payload.size(); ++e) {
            this->_payload.clear();

            auto [entityId, idBytes] = this->_serializer->deserializeVarint(payload, offset);
            offset += idBytes;
            this->_payload.push_back(entityId);

            auto [entitySize, sizeBytes] =
                this->_serializer->deserializeVarint(payload, offset);
            offset += sizeBytes;

            size_t entityEnd = offset + entitySize;
            for (unsigned int i = static_cast<unsigned int>(offset); i < entityEnd && i <
                payload.size();) {
                bool handled = false;
                for (const auto &func : this->_unpackGSFunction) {
                    unsigned int ret = func(payload, i);
                    if (ret > 0) {
                        i += ret;
                        handled = true;
                        break;
                    }
                }
                if (!handled) break;
            }
            offset = entityEnd;

            this->_batchedPayloads.push_back(this->_payload);
        }
        this->_payload.clear();
        return true;
    }

    if (type == GAME_STATE_PACKET) {
        this->_idClient = static_cast<uint8_t>(idClient);
        this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
        this->_type = static_cast<uint8_t>(type);
        this->_length = static_cast<uint32_t>(length);

        std::vector<uint8_t> payload(data.begin() + 11, data.end());
        this->_payload.clear();
        auto [idx, bytesRead] = this->_serializer->deserializeVarint(payload, 0);
        this->_payload.push_back(idx);
        for (unsigned int i = static_cast<unsigned int>(bytesRead); i < payload.size();) {
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

    std::vector<uint8_t> rawPayload(data.begin() + 11, data.end());
    this->_payload.clear();
    for (size_t i = 0; i < rawPayload.size(); i += 8) {
        if (i + 8 > rawPayload.size()) break;
        uint64_t val = this->_serializer->deserializeULong(
            std::vector<uint8_t>(rawPayload.begin() + static_cast<ptrdiff_t>(i),
            rawPayload.begin() + static_cast<ptrdiff_t>(i + 8)));
        this->_payload.push_back(val);
    }
    this->_idClient = static_cast<uint8_t>(idClient);
    this->_sequenceNumber = static_cast<uint32_t>(sequenceNumber);
    this->_type = static_cast<uint8_t>(type);
    this->_length = static_cast<uint32_t>(length);
    return true;
}
