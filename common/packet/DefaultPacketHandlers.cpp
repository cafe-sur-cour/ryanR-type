/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers registration (common)
*/

#include "DefaultPacketHandlers.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "GameStateHandlers.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../libs/Packet/PacketManager.hpp"
#include "../../common/translationToECS.hpp"
#include "../constants.hpp"

using SerializerPtr = std::shared_ptr<pm::ISerializer>;
using pm::BigEndianSerialization;

namespace common::packet {

static SerializerPtr makeSerializer() {
    return std::make_shared<BigEndianSerialization>();
}

static void registerMultiUCharPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id,
    size_t length
) {
    packet->registerBuilder(
        id, [ser, length](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (size_t i = 0; i < length; ++i) {
            auto temp = ser->serializeUChar(payload.at(i));
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(
        id, [ser, packet, length](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != length)
            return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < length; ++i) {
            uint64_t v = ser->deserializeUChar(
                std::vector<uint8_t>(payload.begin() + static_cast<std::ptrdiff_t>(i),
                payload.begin() + static_cast<std::ptrdiff_t>(i + 1))
            );
            vals.push_back(v);
        }
        packet->setPayload(vals);
        return true;
    });
}

static void registerSingleUCharPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id,
    size_t length
) {
    packet->registerBuilder(
        id, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(
        id, [ser, packet, length](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != length)
            return false;
        uint64_t result = ser->deserializeUChar(
            std::vector<uint8_t>(payload.begin(), payload.begin() + 1));
        packet->setPayload(std::vector<uint64_t>{result});
        return true;
    });
}

static void registerUCharULongPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id
) {
    packet->registerBuilder(
        id, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp1 = ser->serializeUChar(payload.at(0));
        body.insert(body.end(), temp1.begin(), temp1.end());
        auto temp2 = ser->serializeULong(payload.at(1));
        body.insert(body.end(), temp2.begin(), temp2.end());
        return body;
    });

    packet->registerParser(
        id, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_EVENT_PACKET)
            return false;
        uint64_t eventType = ser->deserializeUChar(
            std::vector<uint8_t>(payload.begin() + 0, payload.begin() + 1));
        uint64_t param = ser->deserializeULong(
            std::vector<uint8_t>(payload.begin() + 1, payload.begin() + 9));
        packet->setPayload(std::vector<uint64_t>{eventType, param});
        return true;
    });
}

static void registerULongSequencePacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id
) {
    packet->registerBuilder(
        id, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (auto val : payload) {
            auto temp = ser->serializeULong(val);
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(
        id, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() % 8 != 0)
            return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < payload.size(); i += 8) {
            uint64_t value = ser->deserializeULong(
                std::vector<uint8_t>(payload.begin() + static_cast<std::ptrdiff_t>(i),
                payload.begin() + static_cast<std::ptrdiff_t>(i + 8)));
            vals.push_back(value);
        }
        packet->setPayload(vals);
        return true;
    });
}

static void registerSpawnPlayerPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    packet->registerBuilder(
        SPAWN_PLAYER_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        for (size_t i = 1; i < payload.size(); i++) {
            auto t = ser->serializeUChar(payload.at(i));
            body.insert(body.end(), t.begin(), t.end());
        }
        return body;
    });

    packet->registerParser(
        SPAWN_PLAYER_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() < 9)
            return false;
        std::vector<uint64_t> vals;
        uint64_t value = ser->deserializeULong(
            std::vector<uint8_t>(payload.begin(), payload.begin() + 8));
        vals.push_back(value);
        for (size_t i = 8; i < payload.size(); i++) {
            uint64_t charValue = ser->deserializeUChar(
                std::vector<uint8_t>(payload.begin() + static_cast<std::ptrdiff_t>(i),
                payload.begin() + static_cast<std::ptrdiff_t>(i + 1)));
            vals.push_back(charValue);
        }
        packet->setPayload(vals);
        return true;
    });
}

static void registerSingleULongPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id,
    size_t length
) {
    packet->registerBuilder(
        id, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(
        id, [ser, packet, length](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != length)
            return false;
        uint64_t value = ser->deserializeULong(
            std::vector<uint8_t>(payload.begin(), payload.begin() + 8));
        packet->setPayload(std::vector<uint64_t>{value});
        return true;
    });
}

static void registerOptionalULongPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser,
    uint8_t id
) {
    packet->registerBuilder(
        id, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        if (payload.empty())
            return body;
        auto temp = ser->serializeULong(payload.at(0));
        body.insert(body.end(), temp.begin(), temp.end());
        return body;
    });

    packet->registerParser(
        id, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != 0 && payload.size() != 8) return false;
        if (payload.size() == 8) {
            uint64_t value = ser->deserializeULong(
                std::vector<uint8_t>(payload.begin(), payload.begin() + 8));
            packet->setPayload(std::vector<uint64_t>{value});
        } else {
            packet->setPayload(std::vector<uint64_t>());
        }
        return true;
    });
}

static void registerCanStartPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    packet->registerBuilder(
        CAN_START_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        for (auto val : payload) {
            auto temp = ser->serializeULong(val);
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(
        CAN_START_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() < 8 || payload.size() % 8 != 0)
            return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < payload.size(); i += 8) {
            uint64_t value = ser->deserializeULong(
                std::vector<uint8_t>(payload.begin() + static_cast<std::ptrdiff_t>(i),
                payload.begin() + static_cast<std::ptrdiff_t>(i + 8)));
            vals.push_back(value);
        }
        packet->setPayload(vals);
        return true;
    });
}

bool registerDefaultPacketHandlers(
    std::shared_ptr<pm::IPacketManager> packet
) {
    if (!packet)
        return false;
    SerializerPtr ser = makeSerializer();

    auto pushSerialized = [](std::vector<uint8_t> &dst, const std::vector<uint8_t> &bytes) {
        dst.insert(dst.end(), bytes.begin(), bytes.end());
    };

    auto appendUChar = [ser, &pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUChar(val));
    };

    auto appendULong = [ser, &pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeULong(val));
    };

    auto parseUChars =
        [ser, packet](const std::vector<uint8_t> payload, size_t expectedCount) -> bool {
        if (payload.size() != expectedCount)
            return false;
        std::vector<uint64_t> vals;
        for (size_t i = 0; i < expectedCount; ++i) {
            std::vector<uint8_t> charBytes(payload.begin() + static_cast<std::ptrdiff_t>(i),
                payload.begin() + static_cast<std::ptrdiff_t>(i + 1));
            vals.push_back(ser->deserializeUChar(charBytes));
        }
        packet->setPayload(vals);
        return true;
    };

    auto parseULongSequence = [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() % 8 != 0)
            return false;
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
    };

    registerMultiUCharPacket(packet, ser, CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET);
    registerSingleUCharPacket(packet, ser, ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET);
    registerSingleUCharPacket(packet, ser, DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET);

    registerUCharULongPacket(packet, ser, EVENT_PACKET);

    registerULongSequencePacket(packet, ser, MAP_SEND_PACKET);

    registerSingleUCharPacket(packet, ser, END_GAME_PACKET, LENGTH_END_GAME_PACKET);

    registerCanStartPacket(packet, ser);

    registerSpawnPlayerPacket(packet, ser);

    registerSingleULongPacket(packet, ser, DEATH_PLAYER_PACKET, LENGTH_DEATH_PACKET);

    registerOptionalULongPacket(packet, ser, WHOAMI_PACKET);

    registerOptionalULongPacket(packet, ser, constants::PACKET_END_GAME);

    if (!registerGameStateHandlers(packet))
        return false;

    packet->registerLength(CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET);
    packet->registerLength(ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET);
    packet->registerLength(DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET);
    packet->registerLength(EVENT_PACKET, LENGTH_EVENT_PACKET);
    packet->registerLength(DEATH_PLAYER_PACKET, LENGTH_DEATH_PACKET);
    packet->registerLength(WHOAMI_PACKET, LENGTH_WHOAMI_PACKET);
    packet->registerLength(constants::PACKET_END_GAME, 0);

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
    packet->registerLengthCombEntry(PROJECTILE_PREFAB, 0, 2);
    packet->registerLengthCombEntry(NETWORK_ID, 9, 2);
    packet->registerLengthCombEntry(GAME_ZONE, 33, 5);

    packet->registerLengthCombEntry(PLAYER_TAG, 1, 1);
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

    try {
        auto accept = packet->pack(0, 0, ACCEPTATION_PACKET, {0});
        if (accept.empty())
            return false;
        auto conn = packet->pack(
            0, 0, CONNECTION_CLIENT_PACKET,
            std::vector<uint64_t>(LENGTH_CONNECTION_PACKET, 0));
        if (conn.empty())
            return false;
    } catch (...) {
        return false;
    }
    return true;
}

}  // namespace common::packet
