/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Unpack Game State
*/

#include <vector>
#include <memory>
#include "PacketManager.hpp"
#include "../../common/translationToECS.hpp"

unsigned int pm::PacketManager::unpackPlayerTag(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == PLAYER_TAG) {
        this->_payload.push_back(static_cast<uint64_t>(PLAYER_TAG));
        return 1;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackTransform(std::vector<uint8_t> payload, unsigned int i) {
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
        return 41;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackSpeed(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == SPEED) {
        this->_payload.push_back(static_cast<uint64_t>(SPEED));
        uint64_t speed = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        this->_payload.push_back(speed);
        return 9;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackHealth(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == HEALTH) {
        this->_payload.push_back(static_cast<uint64_t>(HEALTH));
        uint64_t health = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        uint64_t baseHealth = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 9,
            payload.begin() + i + 17));
        this->_payload.push_back(health);
        this->_payload.push_back(baseHealth);
        return 17;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackCollider(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == COLLIDER) {
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
        return 34;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackShootingStats(
    std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == SHOOTING_STATS) {
        this->_payload.push_back(static_cast<uint64_t>(SHOOTING_STATS));
        uint64_t fireRate = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        uint64_t cooldownTimer = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 9,
            payload.begin() + i + 17));
        uint64_t shotCount = this->_serializer->deserializeUInt(
            std::vector<uint8_t>(payload.begin() + i + 17,
            payload.begin() + i + 21));
        uint64_t angleSpread = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 21,
            payload.begin() + i + 29));
        uint64_t offsetDistance = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 29,
            payload.begin() + i + 37));
        this->_payload.push_back(fireRate);
        this->_payload.push_back(cooldownTimer);
        this->_payload.push_back(shotCount);
        this->_payload.push_back(angleSpread);
        this->_payload.push_back(offsetDistance);
        return 37;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackScore(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == SCORE) {
        this->_payload.push_back(static_cast<uint64_t>(SCORE));
        uint64_t score = this->_serializer->deserializeUInt(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 5));
        this->_payload.push_back(score);
        return 5;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackAIMovementPattern(
    std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == AI_MOVEMENT_PATTERN) {
        this->_payload.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));
        uint64_t iaMovement = this->_serializer->deserializeUChar(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 2));
        this->_payload.push_back(iaMovement);
        uint64_t zigzagAmplitude = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 2,
            payload.begin() + i + 10));
        this->_payload.push_back(zigzagAmplitude);
        uint64_t zigzagFrequency = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 10,
            payload.begin() + i + 18));
        this->_payload.push_back(zigzagFrequency);
        uint64_t detectionRange = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 18,
            payload.begin() + i + 26));
        this->_payload.push_back(detectionRange);
        uint64_t verticalDeadzone = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 26,
            payload.begin() + i + 34));
        this->_payload.push_back(verticalDeadzone);
        uint64_t timer = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 34,
            payload.begin() + i + 42));
        this->_payload.push_back(timer);
        return 42;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackDamage(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == DAMAGE) {
        this->_payload.push_back(static_cast<uint64_t>(DAMAGE));
        uint64_t damage = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        this->_payload.push_back(damage);
        return 9;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackLifetime(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == LIFETIME) {
        this->_payload.push_back(static_cast<uint64_t>(LIFETIME));
        uint64_t lifetime = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        this->_payload.push_back(lifetime);
        return 9;
    }
    return 0;
}

unsigned int pm::PacketManager::unpackVelocity(std::vector<uint8_t> payload, unsigned int i) {
    if (payload.at(i) == VELOCITY) {
        this->_payload.push_back(static_cast<uint64_t>(VELOCITY));
        uint64_t velX = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 1,
            payload.begin() + i + 9));
        uint64_t velY = this->_serializer->deserializeULong(
            std::vector<uint8_t>(payload.begin() + i + 9,
            payload.begin() + i + 17));
        this->_payload.push_back(velX);
        this->_payload.push_back(velY);
        return 17;
    }
    return 0;
}
