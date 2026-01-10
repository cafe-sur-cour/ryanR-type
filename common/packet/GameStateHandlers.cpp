/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Game state pack/unpack handlers (common)
*/

#include "GameStateHandlers.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "DefaultPacketHandlers.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../common/translationToECS.hpp"

namespace common::packet {

static void registerGameStatePackers(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    auto pushSerialized = [](std::vector<uint8_t> &dst, const std::vector<uint8_t> &bytes) {
        dst.insert(dst.end(), bytes.begin(), bytes.end());
    };
    auto pushUChar = [ser, pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUChar(val));
    };
    auto pushULong = [ser, pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeULong(val));
    };
    auto pushUInt = [ser, pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUInt(val));
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

    /* Transform component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == TRANSFORM) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            pushULong(packetData, payload.at(*i + 3));
            pushULong(packetData, payload.at(*i + 4));
            pushULong(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Speed component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SPEED_COMP) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Health component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == HEALTH) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });

    /* Collider component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == COLLIDER) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            pushULong(packetData, payload.at(*i + 3));
            pushULong(packetData, payload.at(*i + 4));
            pushUChar(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Shooting stats component */
    packet->registerGameStatePackFunction([pushUChar, pushULong, pushUInt](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SHOOTING_STATS) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            pushUInt(packetData, payload.at(*i + 3));
            pushULong(packetData, payload.at(*i + 4));
            pushULong(packetData, payload.at(*i + 5));
            *i += 6;
        }
        return packetData;
    });

    /* Score component */
    packet->registerGameStatePackFunction([pushUChar, pushUInt](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SCORE) {
            pushUChar(packetData, payload.at(*i));
            pushUInt(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* AI movement pattern component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == AI_MOVEMENT_PATTERN) {
            pushUChar(packetData, payload.at(*i));
            pushUChar(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            pushULong(packetData, payload.at(*i + 3));
            pushULong(packetData, payload.at(*i + 4));
            pushULong(packetData, payload.at(*i + 5));
            pushULong(packetData, payload.at(*i + 6));
            *i += 7;
        }
        return packetData;
    });

    /* Damage component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == DAMAGE) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Lifetime component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == LIFETIME) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            *i += 2;
        }
        return packetData;
    });

    /* Velocity component */
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == VELOCITY) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            *i += 3;
        }
        return packetData;
    });

    /* Simple tags */
    auto registerSimpleTag = [packet, ser](uint8_t tag) {
        packet->registerGameStatePackFunction([ser, tag](
            std::vector<uint64_t> payload,
            std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
            std::vector<uint8_t> packetData = {};
            if (payload.at(*i) == tag) {
                auto temp = ser->serializeUChar(payload.at(*i));
                packetData.insert(packetData.end(), temp.begin(), temp.end());
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
    packet->registerGameStatePackFunction([pushUChar, pushULong](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == GAME_ZONE) {
            pushUChar(packetData, payload.at(*i));
            pushULong(packetData, payload.at(*i + 1));
            pushULong(packetData, payload.at(*i + 2));
            pushULong(packetData, payload.at(*i + 3));
            pushULong(packetData, payload.at(*i + 4));
            *i += 5;
        }
        return packetData;
    });

    packet->registerGameStatePackFunction([pushUChar](
        std::vector<uint64_t> payload,
        std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == ANIMATION_STATE) {
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

static void registerGameStateUnpackers(
    std::shared_ptr<pm::IPacketManager> packet,
    SerializerPtr ser
) {
    auto readUCharAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeUChar(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 1));
    };
    auto readULongAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeULong(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 8));
    };
    auto readUIntAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeUInt(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 4));
    };

    /* Player tag */
    packet->registerGameStateUnpackFunction([readUCharAt, packet](
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
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == TRANSFORM) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(TRANSFORM));
            uint64_t posX = readULongAt(payload, i + 1);
            uint64_t posY = readULongAt(payload, i + 9);
            uint64_t rotation = readULongAt(payload, i + 17);
            uint64_t scaleX = readULongAt(payload, i + 25);
            uint64_t scaleY = readULongAt(payload, i + 33);
            vals.push_back(posX);
            vals.push_back(posY);
            vals.push_back(rotation);
            vals.push_back(scaleX);
            vals.push_back(scaleY);
            packet->setPayload(vals);
            return 41;
        }
        return 0;
    });

    /* Speed component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SPEED_COMP) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SPEED_COMP));
            uint64_t speed = readULongAt(payload, i + 1);
            vals.push_back(speed);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    /* Health component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == HEALTH) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(HEALTH));
            uint64_t health = readULongAt(payload, i + 1);
            uint64_t baseHealth = readULongAt(payload, i + 9);
            vals.push_back(health);
            vals.push_back(baseHealth);
            packet->setPayload(vals);
            return 17;
        }
        return 0;
    });

    /* Collider component */
    packet->registerGameStateUnpackFunction([readULongAt, readUCharAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == COLLIDER) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(COLLIDER));
            uint64_t offsetX = readULongAt(payload, i + 1);
            uint64_t offsetY = readULongAt(payload, i + 9);
            uint64_t sizeX = readULongAt(payload, i + 17);
            uint64_t sizeY = readULongAt(payload, i + 25);
            uint64_t collisionType = readUCharAt(payload, i + 33);
            vals.push_back(offsetX);
            vals.push_back(offsetY);
            vals.push_back(sizeX);
            vals.push_back(sizeY);
            vals.push_back(collisionType);
            packet->setPayload(vals);
            return 34;
        }
        return 0;
    });

    /* Shooting stats component */
    packet->registerGameStateUnpackFunction([readULongAt, readUIntAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SHOOTING_STATS) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SHOOTING_STATS));
            uint64_t fireRate = readULongAt(payload, i + 1);
            uint64_t cooldownTimer = readULongAt(payload, i + 9);
            uint64_t shotCount = readUIntAt(payload, i + 17);
            uint64_t angleSpread = readULongAt(payload, i + 21);
            uint64_t offsetDistance = readULongAt(payload, i + 29);
            vals.push_back(fireRate);
            vals.push_back(cooldownTimer);
            vals.push_back(shotCount);
            vals.push_back(angleSpread);
            vals.push_back(offsetDistance);
            packet->setPayload(vals);
            return 37;
        }
        return 0;
    });

    /* Score component */
    packet->registerGameStateUnpackFunction([readUIntAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == SCORE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SCORE));
            uint64_t score = readUIntAt(payload, i + 1);
            vals.push_back(score);
            packet->setPayload(vals);
            return 5;
        }
        return 0;
    });

    /* AI movement pattern component */
    packet->registerGameStateUnpackFunction([readUCharAt, readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == AI_MOVEMENT_PATTERN) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));
            uint64_t iaMovement = readUCharAt(payload, i + 1);
            vals.push_back(iaMovement);
            uint64_t zigzagAmplitude = readULongAt(payload, i + 2);
            vals.push_back(zigzagAmplitude);
            uint64_t zigzagFrequency = readULongAt(payload, i + 10);
            vals.push_back(zigzagFrequency);
            uint64_t detectionRange = readULongAt(payload, i + 18);
            vals.push_back(detectionRange);
            uint64_t verticalDeadzone = readULongAt(payload, i + 26);
            vals.push_back(verticalDeadzone);
            uint64_t timer = readULongAt(payload, i + 34);
            vals.push_back(timer);
            packet->setPayload(vals);
            return 42;
        }
        return 0;
    });

    /* Damage component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == DAMAGE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(DAMAGE));
            uint64_t damage = readULongAt(payload, i + 1);
            vals.push_back(damage);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    /* Lifetime component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == LIFETIME) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(LIFETIME));
            uint64_t lifetime = readULongAt(payload, i + 1);
            vals.push_back(lifetime);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    /* Velocity component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == VELOCITY) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(VELOCITY));
            uint64_t velX = readULongAt(payload, i + 1);
            uint64_t velY = readULongAt(payload, i + 9);
            vals.push_back(velX);
            vals.push_back(velY);
            packet->setPayload(vals);
            return 17;
        }
        return 0;
    });

    /* Simple tags */
    auto registerUnpackSimpleTag = [ser, packet](uint8_t tag) {
        packet->registerGameStateUnpackFunction([ser, packet, tag](
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
    packet->registerGameStateUnpackFunction([readUCharAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == PROJECTILE_PREFAB) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(PROJECTILE_PREFAB));
            std::string prefabName = "";
            unsigned int j = i + 1;
            while (j < payload.size()) {
                char c = static_cast<char>(readUCharAt(payload, j));
                if (c == '\r') {
                    if (j + 2 < payload.size()
                        && static_cast<char>(readUCharAt(payload, j + 1)) == '\n'
                        && static_cast<char>(readUCharAt(payload, j + 2)) == '\0') {
                        j += 3;
                        break;
                    }
                }
                prefabName += c;
                j += 1;
            }
            for (char c : prefabName) {
                vals.push_back(static_cast<uint64_t>(c));
            }
            vals.push_back(static_cast<uint64_t>('\r'));
            vals.push_back(static_cast<uint64_t>('\n'));
            vals.push_back(static_cast<uint64_t>('\0'));
            packet->setPayload(vals);
            return j - i;
        }
        return 0;
    });

    /* Game zone component */
    packet->registerGameStateUnpackFunction([readULongAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == GAME_ZONE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(GAME_ZONE));
            uint64_t height = readULongAt(payload, i + 1);
            uint64_t width = readULongAt(payload, i + 9);
            uint64_t left = readULongAt(payload, i + 17);
            uint64_t top = readULongAt(payload, i + 25);
            vals.push_back(height);
            vals.push_back(width);
            vals.push_back(left);
            vals.push_back(top);
            packet->setPayload(vals);
            return 33;
        }
        return 0;
    });

    packet->registerGameStateUnpackFunction([readUCharAt, packet](
        const std::vector<uint8_t> payload,
        unsigned int i) -> unsigned int {
        if (payload.at(i) == ANIMATION_STATE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(ANIMATION_STATE));
            std::string state = "";
            unsigned int j = i + 1;
            while (j < payload.size()) {
                char c = static_cast<char>(readUCharAt(payload, j));
                if (c == '\r') {
                    if (j + 2 < payload.size()
                        && static_cast<char>(readUCharAt(payload, j + 1)) == '\n'
                        && static_cast<char>(readUCharAt(payload, j + 2)) == '\0') {
                        j += 3;
                        break;
                    }
                }
                state += c;
                j += 1;
            }
            for (char c : state) {
                vals.push_back(static_cast<uint64_t>(c));
            }
            vals.push_back(static_cast<uint64_t>('\r'));
            vals.push_back(static_cast<uint64_t>('\n'));
            vals.push_back(static_cast<uint64_t>('\0'));
            packet->setPayload(vals);
            return j - i;
        }
        return 0;
    });

    /* Register simple tags */
    registerUnpackSimpleTag(AI_MOVER_TAG);
    registerUnpackSimpleTag(AI_SHOOTER_TAG);
    registerUnpackSimpleTag(CONTROLLABLE_TAG);
    registerUnpackSimpleTag(ENEMY_PROJECTILE_TAG);
    registerUnpackSimpleTag(GAME_ZONE_COLLIDER_TAG);
    registerUnpackSimpleTag(MOB_TAG);
    registerUnpackSimpleTag(OBSTACLE_TAG);
    registerUnpackSimpleTag(PLAYER_PROJECTILE_TAG);
    registerUnpackSimpleTag(SHOOTER_TAG);
    registerUnpackSimpleTag(PROJECTILE_PASS_THROUGH_TAG);
}

bool registerGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    if (!packet)
        return false;
    SerializerPtr ser = makeSerializer();

    auto pushSerialized =
        [](std::vector<uint8_t> &dst, const std::vector<uint8_t> &bytes) {
        dst.insert(dst.end(), bytes.begin(), bytes.end());
    };

    auto pushUChar =
        [ser, &pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUChar(val));
    };

    auto pushULong =
        [ser, &pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeULong(val));
    };

    auto pushUInt =
        [ser, &pushSerialized](std::vector<uint8_t> &dst, uint64_t val) {
        pushSerialized(dst, ser->serializeUInt(val));
    };

    auto readUCharAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeUChar(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 1));
    };

    auto readULongAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeULong(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 8));
    };

    auto readUIntAt =
        [ser](const std::vector<uint8_t> &payload, unsigned int offset) -> uint64_t {
        return ser->deserializeUInt(
            std::vector<uint8_t>(payload.begin() + offset, payload.begin() + offset + 4));
    };

    registerGameStatePackers(packet, ser);
    registerGameStateUnpackers(packet, ser);
    return true;
}

}  // namespace common::packet
