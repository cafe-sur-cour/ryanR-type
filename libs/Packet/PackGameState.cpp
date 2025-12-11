/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Pack Game State
*/

#include <vector>
#include <memory>
#include "PacketManager.hpp"
#include "../../common/translationToECS.hpp"

std::vector<uint8_t> pm::PacketManager::packPlayerTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == PLAYER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packTransformation(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == TRANSFORM) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 6;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packSpeed(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SPEED_COMP) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packHealth(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == HEALTH) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 3;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packCollider(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == COLLIDER) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUChar(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 6;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packShootingStats(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SHOOTING_STATS) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUInt(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 6;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packScore(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SCORE) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUInt(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packAIMovementPattern(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == AI_MOVEMENT_PATTERN) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUChar(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 5));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 6));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 7;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packDamage(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == DAMAGE) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packLifetime(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == LIFETIME) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packVelocity(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == VELOCITY) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 3;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packAIMoverTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == AI_MOVER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packAIShooterTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == AI_SHOOTER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packControllableTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == CONTROLLABLE_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packEnemyProjectileTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == ENEMY_PROJECTILE_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packGameZoneColliderTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == GAME_ZONE_COLLIDER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packMobTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == MOB_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packObstacleTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == OBSTACLE_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packPlayerProjectileTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == PLAYER_PROJECTILE_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packScoreTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SCORE_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packShooterTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == SHOOTER_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packProjectilePassThroughTag(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == PROJECTILE_PASS_THROUGH_TAG) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packProjectilePrefabComponent(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == PROJECTILE_PREFAB) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 1;
        while (*i + 2 < payload.size() &&
            !(payload.at(*i)     == static_cast<uint64_t>('\r') &&
            payload.at(*i + 1) == static_cast<uint64_t>('\n') &&
            payload.at(*i + 2) == static_cast<uint64_t>('\0'))) {
            temp = this->_serializer->serializeUChar(payload.at(*i));
            packet.insert(packet.end(), temp.begin(), temp.end());
            *i += 1;
        }
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUChar(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeUChar(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 3;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packNetworkIdComponent(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == NETWORK_ID) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 2;
    }
    return packet;
}

std::vector<uint8_t> pm::PacketManager::packGameZoneComponent(
    std::vector<uint64_t> payload , std::shared_ptr<unsigned int> i) {
    std::vector<uint8_t> temp = {};
    std::vector<uint8_t> packet = {};
    if (payload.at(*i) == GAME_ZONE) {
        temp = this->_serializer->serializeUChar(payload.at(*i));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 1));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 2));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 3));
        packet.insert(packet.end(), temp.begin(), temp.end());
        temp = this->_serializer->serializeULong(payload.at(*i + 4));
        packet.insert(packet.end(), temp.begin(), temp.end());
        *i += 5;
    }
    return packet;
}
