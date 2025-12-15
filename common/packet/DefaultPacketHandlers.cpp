/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers registration (common)
*/

#include "DefaultPacketHandlers.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../libs/Packet/PacketManager.hpp"
#include "../../common/translationToECS.hpp"
#include <iostream>

using namespace pm;
using SerializerPtr = std::shared_ptr<ISerializer>;

namespace common::packet {

static SerializerPtr makeSerializer() {
    return std::make_shared<BigEndianSerialization>();
}

bool registerDefaultPacketHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    if (!packet) return false;
    SerializerPtr ser = makeSerializer();

    packet->registerBuilder(CONNECTION_CLIENT_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (uint8_t i = 0; i < LENGTH_CONNECTION_PACKET; i++) {
            auto temp = ser->serializeUChar(payload.at(i));
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(CONNECTION_CLIENT_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_CONNECTION_PACKET) return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < LENGTH_CONNECTION_PACKET; ++i) {
            std::vector<uint8_t> charBytes(payload.begin() + static_cast<std::ptrdiff_t>(i), payload.begin() + static_cast<std::ptrdiff_t>(i + 1));
            uint64_t v = ser->deserializeUChar(charBytes);
            vals.push_back(v);
        }
        packet->setPayload(vals);
        return true;
    });

    packet->registerBuilder(ACCEPTATION_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(ACCEPTATION_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_ACCEPTATION_PACKET) return false;
        std::vector<uint8_t> charBytes(payload.begin() + 0, payload.begin() + 1);
        uint64_t result = ser->deserializeUChar(charBytes);
        packet->setPayload(std::vector<uint64_t>{result});
        return true;
    });

    packet->registerBuilder(DISCONNECTION_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(DISCONNECTION_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_DISCONNECTION_PACKET) return false;
        std::vector<uint8_t> charBytes(payload.begin() + 0, payload.begin() + 1);
        uint64_t result = ser->deserializeUChar(charBytes);
        packet->setPayload(std::vector<uint64_t>{result});
        return true;
    });

    packet->registerBuilder(EVENT_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp1 = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp1.begin(), temp1.end());
        auto temp2 = ser->serializeULong(payload.at(1));
        body.insert(body.end(), temp2.begin(), temp2.end());
        return body;
    });

    packet->registerParser(EVENT_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_EVENT_PACKET) return false;
        std::vector<uint8_t> charBytes1(payload.begin() + 0, payload.begin() + 1);
        uint64_t eventType = ser->deserializeUChar(charBytes1);
        std::vector<uint8_t> longBytes1(payload.begin() + 1, payload.begin() + 9);
        uint64_t param = ser->deserializeULong(longBytes1);
        packet->setPayload(std::vector<uint64_t>{eventType, param});
        return true;
    });

    packet->registerBuilder(MAP_SEND_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (auto val : payload) {
            auto temp = ser->serializeULong(val);
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(MAP_SEND_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() % 8 != 0) return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < payload.size(); i += 8) {
            auto startIt = payload.begin() + static_cast<std::ptrdiff_t>(i);
            auto endIt = payload.begin() + static_cast<std::ptrdiff_t>(i + 8);
            std::vector<uint8_t> longBytes(startIt, endIt);
            uint64_t value = ser->deserializeULong(longBytes);
            vals.push_back(value);
        }
        packet->setPayload(vals);
        return true;
    });

    packet->registerBuilder(END_GAME_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(END_GAME_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_END_GAME_PACKET) return false;
        std::vector<uint8_t> charBytes(payload.begin() + 0, payload.begin() + 1);
        uint64_t result = ser->deserializeUChar(charBytes);
        packet->setPayload(std::vector<uint64_t>{result});
        return true;
    });

    packet->registerBuilder(CAN_START_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (auto val : payload) {
            auto temp = ser->serializeULong(val);
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(CAN_START_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() < 8 || payload.size() % 8 != 0) return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < payload.size(); i += 8) {
            auto startIt = payload.begin() + static_cast<std::ptrdiff_t>(i);
            auto endIt = payload.begin() + static_cast<std::ptrdiff_t>(i + 8);
            std::vector<uint8_t> longBytes(startIt, endIt);
            uint64_t value = ser->deserializeULong(longBytes);
            vals.push_back(value);
        }
        packet->setPayload(vals);
        return true;
    });

    packet->registerBuilder(SPAWN_PLAYER_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        for (size_t i = 1; i < payload.size(); i++) {
            auto t = ser->serializeUChar(payload.at(i));
            body.insert(body.end(), t.begin(), t.end());
        }
        return body;
    });

    packet->registerParser(SPAWN_PLAYER_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() < 9) return false;
        std::vector<uint64_t> vals;
        auto startIt = payload.begin();
        auto endIt = payload.begin() + 8;
        std::vector<uint8_t> longBytes(startIt, endIt);
        uint64_t value = ser->deserializeULong(longBytes);
        vals.push_back(value);
        for (size_t i = 8; i < payload.size(); i++) {
            std::vector<uint8_t> charBytes(payload.begin() + static_cast<std::ptrdiff_t>(i), payload.begin() + static_cast<std::ptrdiff_t>(i + 1));
            uint64_t charValue = ser->deserializeUChar(charBytes);
            vals.push_back(charValue);
        }
        packet->setPayload(vals);
        return true;
    });

    packet->registerBuilder(DEATH_PLAYER_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(DEATH_PLAYER_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_DEATH_PACKET) return false;
        auto startIt = payload.begin();
        auto endIt = payload.begin() + 8;
        std::vector<uint8_t> longBytes(startIt, endIt);
        uint64_t value = ser->deserializeULong(longBytes);
        packet->setPayload(std::vector<uint64_t>{value});
        return true;
    });

    packet->registerBuilder(WHOAMI_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        if (payload.empty()) return body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(WHOAMI_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != 0 && payload.size() != 8) return false;
        if (payload.size() == 8) {
            auto startIt = payload.begin();
            auto endIt = payload.begin() + 8;
            std::vector<uint8_t> longBytes(startIt, endIt);
            uint64_t value = ser->deserializeULong(longBytes);
            packet->setPayload(std::vector<uint64_t>{value});
        } else {
            packet->setPayload(std::vector<uint64_t>());
        }
        return true;
    });

    packet->registerLength(CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET);
    packet->registerLength(ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET);
    packet->registerLength(DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET);
    packet->registerLength(EVENT_PACKET, LENGTH_EVENT_PACKET);
    packet->registerLength(DEATH_PLAYER_PACKET, LENGTH_DEATH_PACKET);
    packet->registerLength(WHOAMI_PACKET, LENGTH_WHOAMI_PACKET);

    packet->registerLengthCombEntry(PLAYER_TAG, 1, 1);
    packet->registerLengthCombEntry(TRANSFORM, 41, 6);
    packet->registerLengthCombEntry(SPEED_COMP, 9, 2);
    packet->registerLengthCombEntry(HEALTH, 17, 3);
    packet->registerLengthCombEntry(COLLIDER, 34, 6);
    packet->registerLengthCombEntry(SHOOTING_STATS, 37, 6);
    packet->registerLengthCombEntry(SCORE, 5, 2);
    packet->registerLengthCombEntry(AI_MOVEMENT_PATTERN, 42, 7);
    packet->registerLengthCombEntry(DAMAGE, 9, 2);
    packet->registerLengthCombEntry(LIFETIME, 9, 2);
    packet->registerLengthCombEntry(VELOCITY, 17, 3);
    packet->registerLengthCombEntry(AI_MOVER_TAG, 1, 1);
    packet->registerLengthCombEntry(AI_SHOOTER_TAG, 1, 1);
    packet->registerLengthCombEntry(CONTROLLABLE_TAG, 1, 1);
    packet->registerLengthCombEntry(ENEMY_PROJECTILE_TAG, 1, 1);
    packet->registerLengthCombEntry(GAME_ZONE_COLLIDER_TAG, 1, 1);
    packet->registerLengthCombEntry(MOB_TAG, 1, 1);
    packet->registerLengthCombEntry(OBSTACLE_TAG, 1, 1);
    packet->registerLengthCombEntry(PLAYER_PROJECTILE_TAG, 1, 1);
    packet->registerLengthCombEntry(SCORE_TAG, 1, 1);
    packet->registerLengthCombEntry(SHOOTER_TAG, 1, 1);
    packet->registerLengthCombEntry(PROJECTILE_PASS_THROUGH_TAG, 1, 1);
    packet->registerLengthCombEntry(PROJECTILE_PREFAB, 0, 2);
    packet->registerLengthCombEntry(NETWORK_ID, 9, 2);
    packet->registerLengthCombEntry(GAME_ZONE, 33, 5);

    try {
        auto accept = packet->pack(0, 0, ACCEPTATION_PACKET, {0});
        if (accept.empty()) return false;
        auto conn = packet->pack(0, 0, CONNECTION_CLIENT_PACKET, std::vector<uint64_t>(LENGTH_CONNECTION_PACKET, 0));
        if (conn.empty()) return false;
    } catch (...) {
        return false;
    }
    return true;
}

}  // namespace common::packet
