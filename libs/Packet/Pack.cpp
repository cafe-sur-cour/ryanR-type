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
#include "compression/Compression.hpp"
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

    if (type == NEW_CHAT_PACKET || type == BROADCASTED_CHAT_PACKET) {
        length = static_cast<uint32_t>(payload.size());
        size_t n = payload.size();
        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
        for (size_t i = 0; i < n ; ++i) {
            temp = this->_serializer->serializeUChar(payload.at(i));
            packet.insert(packet.end(), temp.begin(), temp.end());
        }
        return packet;
    }

    if (length == 0) {
        if (type == CONNECTION_CLIENT_PACKET || type == CLIENT_READY_PACKET
            || type == REQUEST_LEADERBOARD_PACKET || type == REGISTER_FAIL_PACKET
            || type == REQUEST_PROFILE_PACKET) {
            temp = this->_serializer->serializeUInt(length);
            packet.insert(packet.end(), temp.begin(), temp.end());
            return packet;
        }
        if (type != SPAWN_PLAYER_PACKET &&
            type != CAN_START_PACKET && type != DEATH_PLAYER_PACKET &&
            type != WHOAMI_PACKET && type != END_GAME_PACKET && type != REQUEST_LOBBY_PACKET
            && type != SEND_LOBBY_CODE_PACKET && type != LEVEL_COMPLETE_PACKET
            && type != NEXT_LEVEL_PACKET && type != REQUEST_PROFILE_PACKET
            && type != LEAVE_LOBBY_PACKET) {
            std::cerr << "[PACKET] Error: Unknown packet type "
                << static_cast<int>(type) << " for packing" << std::endl;
            return std::vector<uint8_t>();
        }
        if (type == CAN_START_PACKET) {
            length = static_cast<uint32_t>(payload.size()) * sizeof(uint64_t);
        } else if (type == SPAWN_PLAYER_PACKET) {
            length = static_cast<uint32_t>(payload.size() + 7);
        } else if (type == DEATH_PLAYER_PACKET || type == WHOAMI_PACKET) {
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

std::vector<uint8_t> pm::PacketManager::packBatchedGameState(uint8_t idClient,
    uint32_t sequenceNumber, const std::vector<std::vector<uint64_t>>& entities) {
    std::vector<uint8_t> packet;
    std::vector<uint8_t> temp;

    temp = this->_serializer->serializeUChar(MAGIC_NUMBER);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(idClient);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUInt(sequenceNumber);
    packet.insert(packet.end(), temp.begin(), temp.end());

    temp = this->_serializer->serializeUChar(GAME_STATE_BATCH_PACKET);
    packet.insert(packet.end(), temp.begin(), temp.end());

    std::vector<uint8_t> body;

    size_t validEntityCount = 0;
    for (const auto& entityPayload : entities) {
        if (!entityPayload.empty()) validEntityCount++;
    }

    std::vector<uint8_t> entityCountBytes =
        this->_serializer->serializeVarint(validEntityCount);
    body.insert(body.end(), entityCountBytes.begin(), entityCountBytes.end());

    for (const auto& entityPayload : entities) {
        if (entityPayload.empty()) continue;

        std::vector<uint8_t> entityIdBytes =
            this->_serializer->serializeVarint(entityPayload.at(0));
        body.insert(body.end(), entityIdBytes.begin(), entityIdBytes.end());

        std::vector<uint8_t> entityBody;
        for (uint64_t i = 1; i < entityPayload.size();) {
            auto iPtr = std::make_shared<unsigned int>(static_cast<unsigned int>(i));
            bool handled = false;
            for (auto &func : this->_packGSFunction) {
                std::vector<uint8_t> compData = func(entityPayload, iPtr);
                if (!compData.empty()) {
                    entityBody.insert(entityBody.end(), compData.begin(), compData.end());
                    i = *iPtr;
                    handled = true;
                    break;
                }
            }
            if (!handled) break;
        }

        std::vector<uint8_t> entitySizeBytes =
            this->_serializer->serializeVarint(entityBody.size());
        body.insert(body.end(), entitySizeBytes.begin(), entitySizeBytes.end());
        body.insert(body.end(), entityBody.begin(), entityBody.end());
    }

    std::vector<uint8_t> compressedBody = Compression::compress(body);
    if (!compressedBody.empty() && compressedBody.size() < body.size()) {
        packet[6] = GAME_STATE_BATCH_COMPRESSED_PACKET;
        uint32_t length = static_cast<uint32_t>(compressedBody.size());
        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
        packet.insert(packet.end(), compressedBody.begin(), compressedBody.end());
    } else {
        uint32_t length = static_cast<uint32_t>(body.size());
        temp = this->_serializer->serializeUInt(length);
        packet.insert(packet.end(), temp.begin(), temp.end());
        packet.insert(packet.end(), body.begin(), body.end());
    }

    return packet;
}
