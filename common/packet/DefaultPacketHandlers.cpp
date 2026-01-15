/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Default packet handlers
*/

#include "DefaultPacketHandlers.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include "GameStateHandlersOptimized.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../libs/Packet/PacketManager.hpp"
#include "../../common/translationToECS.hpp"
#include "../constants.hpp"

using SerializerPtr = std::shared_ptr<pm::ISerializer>;
using pm::BigEndianSerialization;


namespace common::packet {

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

static void registerServerStatusPacket(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    packet->registerBuilder(
        SERVER_STATUS_PACKET, [ser](std::vector<uint64_t> payload) -> std::vector<uint8_t> {
        std::vector<uint8_t> body;
        size_t nbDataPayload = 4;
        for (size_t i = 0; i < nbDataPayload && i < payload.size(); ++i) {
            auto temp = ser->serializeULong(payload.at(i));
            body.insert(body.end(), temp.begin(), temp.end());
        }
        return body;
    });

    packet->registerParser(
        SERVER_STATUS_PACKET, [ser, packet](const std::vector<uint8_t> payload) -> bool {
        if (payload.size() != LENGTH_SERVER_STATUS_PACKET)
            return false;
        std::vector<uint64_t> vals;
        size_t nbDataPayload = 4;
        for (size_t i = 0; i < nbDataPayload; i++) {
            uint64_t value = ser->deserializeULong(
                std::vector<uint8_t>(payload.begin() + static_cast<std::ptrdiff_t>(i * 8),
                payload.begin() + static_cast<std::ptrdiff_t>((i + 1) * 8)));
            vals.push_back(value);
        }
        packet->setPayload(vals);
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

    auto appendUChar = [ser, pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUChar(val));
    };

    auto appendULong = [ser, pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
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

    registerOptionalULongPacket(packet, ser, CONNECTION_CLIENT_PACKET);
    registerOptionalULongPacket(packet, ser, REQUEST_LEADERBOARD_PACKET);
    registerOptionalULongPacket(packet, ser, REQUEST_PROFILE_PACKET);
    registerSingleUCharPacket(packet, ser, ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET);
    registerSingleUCharPacket(packet, ser, DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET);
    registerSingleUCharPacket(packet, ser, LEAVE_LOBBY_PACKET, LENGTH_DISCONNECTION_PACKET);

    registerUCharULongPacket(packet, ser, EVENT_PACKET);

    registerSingleUCharPacket(packet, ser, END_GAME_PACKET, LENGTH_END_GAME_PACKET);

    registerCanStartPacket(packet, ser);

    registerSpawnPlayerPacket(packet, ser);

    registerSingleULongPacket(packet, ser, DEATH_PLAYER_PACKET, LENGTH_DEATH_PACKET);

    registerOptionalULongPacket(packet, ser, WHOAMI_PACKET);

    registerServerStatusPacket(packet, ser);

    registerOptionalULongPacket(packet, ser, END_GAME_PACKET);


    if (!registerOptimizedGameStateHandlers(packet))
        return false;

    registerOptionalULongPacket(packet, ser, REQUEST_LOBBY_PACKET);
    registerOptionalULongPacket(packet, ser, REGISTER_FAIL_PACKET);
    registerMultiUCharPacket(packet, ser, SEND_LOBBY_CODE_PACKET, LENGTH_LOBBY_CODE_PACKET);
    registerMultiUCharPacket(packet, ser, REGISTER_PACKET, LENGTH_REGISTER_PACKET);
    registerMultiUCharPacket(packet, ser, LOGIN_PACKET, LENGTH_LOGIN_PACKET);
    registerMultiUCharPacket(packet, ser, CONNECT_TO_LOBBY, LENGTH_LOBBY_CODE_PACKET);
    registerMultiUCharPacket(packet, ser, LOBBY_MASTER_REQUEST_START,
        LENGTH_LOBBY_CODE_PACKET);
    registerSingleUCharPacket(packet, ser, LOBBY_CONNECT_VALUE,
        LENGTH_CONNECT_TO_LOBBY_PACKET);
    registerSingleUCharPacket(packet, ser, ACK_LEAVE_LOBBY,
        LENGTH_CONNECT_TO_LOBBY_PACKET);
    registerOptionalULongPacket(packet, ser, LEVEL_COMPLETE_PACKET);
    registerOptionalULongPacket(packet, ser, NEXT_LEVEL_PACKET);
    registerMultiUCharPacket(packet, ser, CONNECT_USER_PACKET, LENGTH_CONNECT_USER_PACKET);
    registerMultiUCharPacket(packet, ser, LEADERBOARD_PACKET, LENGTH_LEADERBOARD_PACKET);
    registerMultiUCharPacket(packet, ser, PROFILE_PACKET, LENGTH_PROFILE_PACKET);
    registerMultiUCharPacket(packet, ser, GAME_RULES_PACKET, LENGTH_GAME_RULES_PACKET);
    registerMultiUCharPacket(packet, ser,
        REQUEST_GAME_RULES_UPDATE_PACKET, LENGTH_REQUEST_GAME_RULES_UPDATE_PACKET);

    registerSingleUCharPacket(packet, ser, FORCE_LEAVE_PACKET, LENGTH_FORCE_LEAVE_PACKET);

    packet->registerLength(FORCE_LEAVE_PACKET, LENGTH_FORCE_LEAVE_PACKET);

    packet->registerLength(LOBBY_CONNECT_VALUE, LENGTH_CONNECT_TO_LOBBY_PACKET);
    packet->registerLength(ACK_LEAVE_LOBBY, LENGTH_CONNECT_TO_LOBBY_PACKET);
    packet->registerLength(REQUEST_LOBBY_PACKET, LENGTH_REQUEST_LOBBY_PACKET);
    packet->registerLength(SEND_LOBBY_CODE_PACKET, LENGTH_LOBBY_CODE_PACKET);
    packet->registerLength(CONNECT_TO_LOBBY, LENGTH_LOBBY_CODE_PACKET);
    packet->registerLength(LOBBY_MASTER_REQUEST_START, LENGTH_LOBBY_CODE_PACKET);
    packet->registerLength(REGISTER_FAIL_PACKET, LENGTH_FAIL_REGISTER_PACKET);

    packet->registerLength(CONNECTION_CLIENT_PACKET, LENGTH_CONNECTION_PACKET);
    packet->registerLength(ACCEPTATION_PACKET, LENGTH_ACCEPTATION_PACKET);
    packet->registerLength(DISCONNECTION_PACKET, LENGTH_DISCONNECTION_PACKET);
    packet->registerLength(EVENT_PACKET, LENGTH_EVENT_PACKET);
    packet->registerLength(DEATH_PLAYER_PACKET, LENGTH_DEATH_PACKET);
    packet->registerLength(WHOAMI_PACKET, LENGTH_WHOAMI_PACKET);
    packet->registerLength(SERVER_STATUS_PACKET, LENGTH_SERVER_STATUS_PACKET);
    packet->registerLength(END_GAME_PACKET, LENGTH_END_GAME_PACKET);
    packet->registerLength(REGISTER_PACKET, LENGTH_REGISTER_PACKET);
    packet->registerLength(CONNECT_USER_PACKET, LENGTH_CONNECT_USER_PACKET);
    packet->registerLength(LOGIN_PACKET, LENGTH_LOGIN_PACKET);
    packet->registerLength(REQUEST_LEADERBOARD_PACKET, LENGTH_REQUEST_LEADERBOARD_PACKET);
    packet->registerLength(LEADERBOARD_PACKET, LENGTH_LEADERBOARD_PACKET);
    packet->registerLength(REQUEST_PROFILE_PACKET, LENGTH_REQUEST_PROFILE_PACKET);
    packet->registerLength(PROFILE_PACKET, LENGTH_PROFILE_PACKET);
    packet->registerLength(GAME_RULES_PACKET, LENGTH_GAME_RULES_PACKET);
    packet->registerLength(
        REQUEST_GAME_RULES_UPDATE_PACKET, LENGTH_REQUEST_GAME_RULES_UPDATE_PACKET);
    packet->registerLength(LEAVE_LOBBY_PACKET, LENGTH_DISCONNECTION_PACKET);

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
