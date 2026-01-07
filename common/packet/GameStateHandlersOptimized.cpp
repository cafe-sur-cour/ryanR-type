/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Optimized Game state handlers using varint encoding
*/

#include "GameStateHandlersOptimized.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../libs/Packet/serializer/Varint.hpp"
#include "../../common/translationToECS.hpp"

using SerializerPtr = std::shared_ptr<pm::ISerializer>;
using pm::BigEndianSerialization;
using pm::Varint;

namespace common::packet {

static SerializerPtr makeSerializer() {
    return std::make_shared<BigEndianSerialization>();
}

static void registerOptimizedGameStatePackers(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    auto pushVarint = [](std::vector<uint8_t> &dst, uint64_t val) {
        auto encoded = Varint::encode(val);
        dst.insert(dst.end(), encoded.begin(), encoded.end());
    };

    auto pushSignedVarint = [](std::vector<uint8_t> &dst, int64_t val) {
        auto encoded = Varint::encodeSigned(val);
        dst.insert(dst.end(), encoded.begin(), encoded.end());
    };

    auto pushUChar = [ser](std::vector<uint8_t> &dst, uint64_t val) {
        auto temp = ser->serializeUChar(val);
        dst.insert(dst.end(), temp.begin(), temp.end());
    };

    /* Player tag */
    packet->registerGameStatePackFunction([pushUChar](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == PLAYER_TAG) {
            pushUChar(packetData, payload.at(*i));
            *i += 1;
        }
        return packetData;
    });

    /* Transform component - uses varint for positions */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == TRANSFORM) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            pushVarint(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Speed component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SPEED_COMP) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Health component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == HEALTH) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });

    /* Collider component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == COLLIDER) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            pushUChar(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Shooting stats component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SHOOTING_STATS) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            pushVarint(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Score component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SCORE) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* AI movement pattern component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == AI_MOVEMENT_PATTERN) {
            pushUChar(packetData, payload.at(*i));
            pushUChar(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            pushVarint(packetData, payload.at(*i + 5));
            pushVarint(packetData, payload.at(*i + 6));
            *i += 7;
        }
        return packetData;
    });

    /* Damage component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == DAMAGE) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Lifetime component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == LIFETIME) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Velocity component - uses signed varint for negative velocities */
    packet->registerGameStatePackFunction([pushUChar, pushSignedVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == VELOCITY) {
            pushUChar(packetData, payload.at(*i));
            pushSignedVarint(packetData, static_cast<int64_t>(payload.at(*i + 1)));
            pushSignedVarint(packetData, static_cast<int64_t>(payload.at(*i + 2)));
            *i += 3;
        }
        return packetData;
    });

    /* Simple tags - just single byte */
    auto registerSimpleTag = [packet, pushUChar](uint8_t tag) {
        packet->registerGameStatePackFunction([pushUChar, tag](
            std::vector<uint64_t> payload,
            std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
            std::vector<uint8_t> packetData = {};
            if (payload.at(*i) == tag) {
                pushUChar(packetData, payload.at(*i));
                *i += 1;
            }
            return packetData;
        });
    };

    /* Projectile prefab component */
    packet->registerGameStatePackFunction([pushUChar](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == PROJECTILE_PREFAB) {
            pushUChar(packetData, payload.at(*i));
            *i += 1;
            while (*i + 2 < payload.size()
                && !(payload.at(*i) == static_cast<uint64_t>('\r')
                && payload.at(*i + 1) == static_cast<uint64_t>('\n')
                && payload.at(*i + 2) == static_cast<uint64_t>('\0'))) {
                pushUChar(packetData, payload.at(*i));
                *i += 1;
            }
            pushUChar(packetData, payload.at(*i));
            pushUChar(packetData, payload.at(*i + 1));
            pushUChar(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });

    /* Game zone component */
    packet->registerGameStatePackFunction([pushUChar, pushVarint](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == GAME_ZONE) {
            pushUChar(packetData, payload.at(*i));
            pushVarint(packetData, payload.at(*i + 1));
            pushVarint(packetData, payload.at(*i + 2));
            pushVarint(packetData, payload.at(*i + 3));
            pushVarint(packetData, payload.at(*i + 4));
            *i += 5;
        }
        return packetData;
    });

    /* Register simple tags */
    registerSimpleTag(AI_MOVER_TAG);
    registerSimpleTag(AI_SHOOTER_TAG);
    registerSimpleTag(CONTROLLABLE_TAG);
    registerSimpleTag(ENEMY_PROJECTILE_TAG);
    registerSimpleTag(GAME_ZONE_COLLIDER_TAG);
    registerSimpleTag(MOB_TAG);
    registerSimpleTag(OBSTACLE_TAG);
    registerSimpleTag(PLAYER_PROJECTILE_TAG);
    registerSimpleTag(SHOOTER_TAG);
    registerSimpleTag(PROJECTILE_PASS_THROUGH_TAG);
}

static void registerOptimizedGameStateUnpackers(
    std::shared_ptr<pm::IPacketManager> packet,
    [[maybe_unused]] SerializerPtr ser
) {
    auto readVarintAt =
        [](const std::vector<uint8_t> &payload, unsigned int offset)
            -> std::pair<uint64_t, size_t> {
        return Varint::decode(payload, offset);
    };

    auto readSignedVarintAt =
        [](const std::vector<uint8_t> &payload, unsigned int offset)
            -> std::pair<int64_t, size_t> {
        return Varint::decodeSigned(payload, offset);
    };

    /* Player tag */
    packet->registerGameStateUnpackFunction([packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == PLAYER_TAG) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(PLAYER_TAG));
            packet->setPayload(vals);
            return 1;
        }
        return 0;
    });

    /* Transform component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == TRANSFORM) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(TRANSFORM));

            size_t offset = i + 1;
            auto [posX, s1] = readVarintAt(payload, offset); offset += s1;
            auto [posY, s2] = readVarintAt(payload, offset); offset += s2;
            auto [rotation, s3] = readVarintAt(payload, offset); offset += s3;
            auto [scaleX, s4] = readVarintAt(payload, offset); offset += s4;
            auto [scaleY, s5] = readVarintAt(payload, offset); offset += s5;

            vals.push_back(posX);
            vals.push_back(posY);
            vals.push_back(rotation);
            vals.push_back(scaleX);
            vals.push_back(scaleY);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Speed component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SPEED_COMP) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SPEED_COMP));

            auto [speed, s1] = readVarintAt(payload, i + 1);
            vals.push_back(speed);
            packet->setPayload(vals);
            return static_cast<unsigned int>(1 + s1);
        }
        return 0;
    });

    /* Health component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == HEALTH) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(HEALTH));

            size_t offset = i + 1;
            auto [health, s1] = readVarintAt(payload, offset); offset += s1;
            auto [baseHealth, s2] = readVarintAt(payload, offset); offset += s2;

            vals.push_back(health);
            vals.push_back(baseHealth);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Collider component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == COLLIDER) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(COLLIDER));

            size_t offset = i + 1;
            auto [offsetX, s1] = readVarintAt(payload, offset); offset += s1;
            auto [offsetY, s2] = readVarintAt(payload, offset); offset += s2;
            auto [sizeX, s3] = readVarintAt(payload, offset); offset += s3;
            auto [sizeY, s4] = readVarintAt(payload, offset); offset += s4;
            uint64_t collisionType = payload.at(offset); offset += 1;

            vals.push_back(offsetX);
            vals.push_back(offsetY);
            vals.push_back(sizeX);
            vals.push_back(sizeY);
            vals.push_back(collisionType);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Shooting stats component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SHOOTING_STATS) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SHOOTING_STATS));

            size_t offset = i + 1;
            auto [fireRate, s1] = readVarintAt(payload, offset); offset += s1;
            auto [cooldownTimer, s2] = readVarintAt(payload, offset); offset += s2;
            auto [shotCount, s3] = readVarintAt(payload, offset); offset += s3;
            auto [angleSpread, s4] = readVarintAt(payload, offset); offset += s4;
            auto [offsetDistance, s5] = readVarintAt(payload, offset); offset += s5;

            vals.push_back(fireRate);
            vals.push_back(cooldownTimer);
            vals.push_back(shotCount);
            vals.push_back(angleSpread);
            vals.push_back(offsetDistance);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Score component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SCORE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SCORE));

            auto [score, s1] = readVarintAt(payload, i + 1);
            vals.push_back(score);
            packet->setPayload(vals);
            return static_cast<unsigned int>(1 + s1);
        }
        return 0;
    });

    /* AI movement pattern component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == AI_MOVEMENT_PATTERN) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));

            size_t offset = i + 1;
            uint64_t patternType = payload.at(offset); offset += 1;
            auto [v1, s1] = readVarintAt(payload, offset); offset += s1;
            auto [v2, s2] = readVarintAt(payload, offset); offset += s2;
            auto [v3, s3] = readVarintAt(payload, offset); offset += s3;
            auto [v4, s4] = readVarintAt(payload, offset); offset += s4;
            auto [v5, s5] = readVarintAt(payload, offset); offset += s5;

            vals.push_back(patternType);
            vals.push_back(v1);
            vals.push_back(v2);
            vals.push_back(v3);
            vals.push_back(v4);
            vals.push_back(v5);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Damage component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == DAMAGE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(DAMAGE));

            auto [damage, s1] = readVarintAt(payload, i + 1);
            vals.push_back(damage);
            packet->setPayload(vals);
            return static_cast<unsigned int>(1 + s1);
        }
        return 0;
    });

    /* Lifetime component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == LIFETIME) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(LIFETIME));

            auto [lifetime, s1] = readVarintAt(payload, i + 1);
            vals.push_back(lifetime);
            packet->setPayload(vals);
            return static_cast<unsigned int>(1 + s1);
        }
        return 0;
    });

    /* Velocity component */
    packet->registerGameStateUnpackFunction([readSignedVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == VELOCITY) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(VELOCITY));

            size_t offset = i + 1;
            auto [velX, s1] = readSignedVarintAt(payload, offset); offset += s1;
            auto [velY, s2] = readSignedVarintAt(payload, offset); offset += s2;

            vals.push_back(static_cast<uint64_t>(velX));
            vals.push_back(static_cast<uint64_t>(velY));
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Simple tags */
    auto registerSimpleTagUnpacker = [packet](uint8_t tag) {
        packet->registerGameStateUnpackFunction([packet, tag](
            const std::vector<uint8_t> payload,
            unsigned int i) -> unsigned int {
            if (payload.at(i) == tag) {
                auto vals = packet->getPayload();
                vals.push_back(static_cast<uint64_t>(tag));
                packet->setPayload(vals);
                return 1;
            }
            return 0;
        });
    };

    /* Projectile prefab component */
    packet->registerGameStateUnpackFunction([packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == PROJECTILE_PREFAB) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(PROJECTILE_PREFAB));
            unsigned int offset = i + 1;
            while (offset + 2 < payload.size()
                && !(payload.at(offset) == static_cast<uint8_t>('\r')
                && payload.at(offset + 1) == static_cast<uint8_t>('\n')
                && payload.at(offset + 2) == static_cast<uint8_t>('\0'))) {
                vals.push_back(static_cast<uint64_t>(payload.at(offset)));
                offset++;
            }
            vals.push_back(static_cast<uint64_t>(payload.at(offset)));
            vals.push_back(static_cast<uint64_t>(payload.at(offset + 1)));
            vals.push_back(static_cast<uint64_t>(payload.at(offset + 2)));
            packet->setPayload(vals);
            return (offset + 3) - i;
        }
        return 0;
    });

    /* Game zone component */
    packet->registerGameStateUnpackFunction([readVarintAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == GAME_ZONE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(GAME_ZONE));

            size_t offset = i + 1;
            auto [v1, s1] = readVarintAt(payload, offset); offset += s1;
            auto [v2, s2] = readVarintAt(payload, offset); offset += s2;
            auto [v3, s3] = readVarintAt(payload, offset); offset += s3;
            auto [v4, s4] = readVarintAt(payload, offset); offset += s4;

            vals.push_back(v1);
            vals.push_back(v2);
            vals.push_back(v3);
            vals.push_back(v4);
            packet->setPayload(vals);
            return static_cast<unsigned int>(offset - i);
        }
        return 0;
    });

    /* Register simple tag unpackers */
    registerSimpleTagUnpacker(AI_MOVER_TAG);
    registerSimpleTagUnpacker(AI_SHOOTER_TAG);
    registerSimpleTagUnpacker(CONTROLLABLE_TAG);
    registerSimpleTagUnpacker(ENEMY_PROJECTILE_TAG);
    registerSimpleTagUnpacker(GAME_ZONE_COLLIDER_TAG);
    registerSimpleTagUnpacker(MOB_TAG);
    registerSimpleTagUnpacker(OBSTACLE_TAG);
    registerSimpleTagUnpacker(PLAYER_PROJECTILE_TAG);
    registerSimpleTagUnpacker(SHOOTER_TAG);
    registerSimpleTagUnpacker(PROJECTILE_PASS_THROUGH_TAG);
}

bool registerOptimizedGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    auto ser = makeSerializer();
    registerOptimizedGameStatePackers(packet, ser);
    registerOptimizedGameStateUnpackers(packet, ser);
    return true;
}

bool isOptimizedHandlersAvailable() {
    return true;
}

}  //  namespace common::packet
