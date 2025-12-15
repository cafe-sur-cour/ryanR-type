/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Game state pack/unpack handlers (common)
*/

#include "GameStateHandlers.hpp"
#include "DefaultPacketHandlers.hpp"
#include "../../libs/Packet/serializer/BigEndianSerialization.hpp"
#include "../../common/translationToECS.hpp"
#include <iostream>

using namespace pm;
using SerializerPtr = std::shared_ptr<ISerializer>;

namespace common::packet {

static SerializerPtr makeSerializer() {
    return std::make_shared<BigEndianSerialization>();
}

bool registerGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet) {
    if (!packet) return false;
    SerializerPtr ser = makeSerializer();

    // Pack handlers (register each component pack function)
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> temp = {};
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == PLAYER_TAG) {
            temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 1;
        }
        return packetData;
    });

    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> temp = {};
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == TRANSFORM) {
            temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 3));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 4));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 5));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 6;
        }
        return packetData;
    });

    // ... register remaining pack functions similarly (speed, health, collider, ...)

    // To avoid duplicating a lot of code here, register the rest in a compact form
    // using helper lambdas that capture the serializer and expected layout.

    // Speed
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SPEED_COMP) {
            auto temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 2;
        }
        return packetData;
    });

    // Health
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == HEALTH) {
            auto temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 3;
        }
        return packetData;
    });

    // Collider
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == COLLIDER) {
            auto temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 3));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 4));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUChar(payload.at(*i + 5));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 6;
        }
        return packetData;
    });

    // Shooting stats
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SHOOTING_STATS) {
            auto temp = ser->serializeUChar(payload.at(*i));
            packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUInt(payload.at(*i + 3)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 4)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 5)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 6;
        }
        return packetData;
    });

    // Score
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == SCORE) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUInt(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 2;
        }
        return packetData;
    });

    // AI movement pattern
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == AI_MOVEMENT_PATTERN) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUChar(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 3)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 4)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 5)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 6)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 7;
        }
        return packetData;
    });

    // Damage, Lifetime, Velocity
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == DAMAGE) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 2;
        }
        if (payload.at(*i) == LIFETIME) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 2;
        }
        if (payload.at(*i) == VELOCITY) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 3;
        }
        return packetData;
    });

    // Simple 1-byte tags
    auto registerSimpleTag = [packet, ser](uint8_t tag) {
        packet->registerGameStatePackFunction([ser, tag](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
            std::vector<uint8_t> packetData = {};
            if (payload.at(*i) == tag) {
                auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
                *i += 1;
            }
            return packetData;
        });
    };

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

    // Projectile prefab (string terminated by \r\n\0)
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == PROJECTILE_PREFAB) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 1;
            while (*i + 2 < payload.size() &&
                !(payload.at(*i) == static_cast<uint64_t>('\r') &&
                payload.at(*i + 1) == static_cast<uint64_t>('\n') &&
                payload.at(*i + 2) == static_cast<uint64_t>('\0'))) {
                temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
                *i += 1;
            }
            temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUChar(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeUChar(payload.at(*i + 2)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 3;
        }
        return packetData;
    });

    // Network id
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == NETWORK_ID) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 2;
        }
        return packetData;
    });

    // Game zone
    packet->registerGameStatePackFunction([ser](std::vector<uint64_t> payload, std::shared_ptr<unsigned int> i) -> std::vector<uint8_t> {
        std::vector<uint8_t> packetData = {};
        if (payload.at(*i) == GAME_ZONE) {
            auto temp = ser->serializeUChar(payload.at(*i)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 1)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 2)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 3)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            temp = ser->serializeULong(payload.at(*i + 4)); packetData.insert(packetData.end(), temp.begin(), temp.end());
            *i += 5;
        }
        return packetData;
    });

    // Unpack handlers (each function should append to packet payload by getting current payload and pushing values)
    // Player tag
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == PLAYER_TAG) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(PLAYER_TAG));
            packet->setPayload(vals);
            return 1;
        }
        return 0;
    });

    // Transform
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == TRANSFORM) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(TRANSFORM));
            uint64_t posX = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t posY = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            uint64_t rotation = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 17, payload.begin() + i + 25));
            uint64_t scaleX = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 25, payload.begin() + i + 33));
            uint64_t scaleY = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 33, payload.begin() + i + 41));
            vals.push_back(posX); vals.push_back(posY); vals.push_back(rotation); vals.push_back(scaleX); vals.push_back(scaleY);
            packet->setPayload(vals);
            return 41;
        }
        return 0;
    });

    // Speed
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == SPEED_COMP) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SPEED_COMP));
            uint64_t speed = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            vals.push_back(speed);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    // Health
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == HEALTH) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(HEALTH));
            uint64_t health = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t baseHealth = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            vals.push_back(health); vals.push_back(baseHealth);
            packet->setPayload(vals);
            return 17;
        }
        return 0;
    });

    // Collider
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == COLLIDER) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(COLLIDER));
            uint64_t offsetX = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t offsetY = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            uint64_t sizeX = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 17, payload.begin() + i + 25));
            uint64_t sizeY = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 25, payload.begin() + i + 33));
            uint64_t Collisiontype = ser->deserializeUChar(std::vector<uint8_t>(payload.begin() + i + 33, payload.begin() + i + 34));
            vals.push_back(offsetX); vals.push_back(offsetY); vals.push_back(sizeX); vals.push_back(sizeY); vals.push_back(Collisiontype);
            packet->setPayload(vals);
            return 34;
        }
        return 0;
    });

    // Shooting stats
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == SHOOTING_STATS) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SHOOTING_STATS));
            uint64_t fireRate = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t cooldownTimer = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            uint64_t shotCount = ser->deserializeUInt(std::vector<uint8_t>(payload.begin() + i + 17, payload.begin() + i + 21));
            uint64_t angleSpread = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 21, payload.begin() + i + 29));
            uint64_t offsetDistance = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 29, payload.begin() + i + 37));
            vals.push_back(fireRate); vals.push_back(cooldownTimer); vals.push_back(shotCount); vals.push_back(angleSpread); vals.push_back(offsetDistance);
            packet->setPayload(vals);
            return 37;
        }
        return 0;
    });

    // Score
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == SCORE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(SCORE));
            uint64_t score = ser->deserializeUInt(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 5));
            vals.push_back(score);
            packet->setPayload(vals);
            return 5;
        }
        return 0;
    });

    // AI movement pattern
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == AI_MOVEMENT_PATTERN) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));
            uint64_t iaMovement = ser->deserializeUChar(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 2));
            vals.push_back(iaMovement);
            uint64_t zigzagAmplitude = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 2, payload.begin() + i + 10));
            vals.push_back(zigzagAmplitude);
            uint64_t zigzagFrequency = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 10, payload.begin() + i + 18));
            vals.push_back(zigzagFrequency);
            uint64_t detectionRange = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 18, payload.begin() + i + 26));
            vals.push_back(detectionRange);
            uint64_t verticalDeadzone = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 26, payload.begin() + i + 34));
            vals.push_back(verticalDeadzone);
            uint64_t timer = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 34, payload.begin() + i + 42));
            vals.push_back(timer);
            packet->setPayload(vals);
            return 42;
        }
        return 0;
    });

    // Damage
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == DAMAGE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(DAMAGE));
            uint64_t damage = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            vals.push_back(damage);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    // Lifetime
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == LIFETIME) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(LIFETIME));
            uint64_t lifetime = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            vals.push_back(lifetime);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    // Velocity
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == VELOCITY) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(VELOCITY));
            uint64_t velX = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t velY = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            vals.push_back(velX); vals.push_back(velY);
            packet->setPayload(vals);
            return 17;
        }
        return 0;
    });

    // Simple tags (1 byte)
    auto registerUnpackSimpleTag = [ser, packet](uint8_t tag) {
        packet->registerGameStateUnpackFunction([ser, packet, tag](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
            if (payload.at(i) == tag) {
                auto vals = packet->getPayload();
                vals.push_back(static_cast<uint64_t>(tag));
                packet->setPayload(vals);
                return 1;
            }
            return 0;
        });
    };

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

    // Projectile prefab
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == PROJECTILE_PREFAB) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(PROJECTILE_PREFAB));
            std::string prefabName = "";
            unsigned int j = i + 1;
            while (j < payload.size()) {
                char c = static_cast<char>(ser->deserializeUChar(std::vector<uint8_t>(payload.begin() + j, payload.begin() + j + 1)));
                if (c == '\r') {
                    if (j + 2 < payload.size() &&
                        static_cast<char>(ser->deserializeUChar(std::vector<uint8_t>(payload.begin() + j + 1, payload.begin() + j + 2))) == '\n' &&
                        static_cast<char>(ser->deserializeUChar(std::vector<uint8_t>(payload.begin() + j + 2, payload.begin() + j + 3))) == '\0') {
                        j += 3;
                        break;
                    }
                }
                prefabName += c;
                j += 1;
            }
            for (char c : prefabName) vals.push_back(static_cast<uint64_t>(c));
            vals.push_back(static_cast<uint64_t>('\r'));
            vals.push_back(static_cast<uint64_t>('\n'));
            vals.push_back(static_cast<uint64_t>('\0'));
            packet->setPayload(vals);
            return j - i;
        }
        return 0;
    });

    // Network id
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == NETWORK_ID) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(NETWORK_ID));
            uint64_t networkId = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            vals.push_back(networkId);
            packet->setPayload(vals);
            return 9;
        }
        return 0;
    });

    // Game zone
    packet->registerGameStateUnpackFunction([ser, packet](const std::vector<uint8_t> payload, unsigned int i) -> unsigned int {
        if (payload.at(i) == GAME_ZONE) {
            auto vals = packet->getPayload();
            vals.push_back(static_cast<uint64_t>(GAME_ZONE));
            uint64_t height = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 1, payload.begin() + i + 9));
            uint64_t width = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 9, payload.begin() + i + 17));
            uint64_t left = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 17, payload.begin() + i + 25));
            uint64_t top = ser->deserializeULong(std::vector<uint8_t>(payload.begin() + i + 25, payload.begin() + i + 33));
            vals.push_back(height); vals.push_back(width); vals.push_back(left); vals.push_back(top);
            packet->setPayload(vals);
            return 33;
        }
        return 0;
    });

    return true;
}

} // namespace common::packet
