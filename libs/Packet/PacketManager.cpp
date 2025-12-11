/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Packet
*/

#include <vector>
#include <map>
#include <iostream>
#include <memory>
#include <string>
#include "PacketManager.hpp"
#include "serializer/BigEndianSerialization.hpp"
#include "../../common/DLLoader/LoaderType.hpp"
#include "../../common/Error/PacketError.hpp"
#include "../../common/translationToECS.hpp"

pm::PacketManager::PacketManager(uint32_t seqNumber) {
    this->_idClient = 0;
    this->_sequenceNumber = seqNumber;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_payload = std::vector<uint64_t>();
    this->_serializer = std::make_shared<BigEndianSerialization>();
    if (!this->_serializer) {
        throw err::PacketError(
            "[Server] Failed to load serializer",
            err::PacketError::SERIALIZER_ATTRIBUTION_FAILED
        );
    }

    this->_packetHandlers = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET), std::bind(
                &pm::PacketManager::buildConnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET), std::bind(
                &pm::PacketManager::buildAcceptationPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET), std::bind(
                &pm::PacketManager::buildDisconnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(EVENT_PACKET), std::bind(
            &pm::PacketManager::buildEventPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(MAP_SEND_PACKET), std::bind(
            &pm::PacketManager::buildMapPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(CAN_START_PACKET), std::bind(
            &pm::PacketManager::buildCanStartPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(SPAWN_PLAYER_PACKET), std::bind(
            &pm::PacketManager::buildSpawnPlayerPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(DEATH_PLAYER_PACKET), std::bind(
            &pm::PacketManager::buildDeathPacket,
            this, std::placeholders::_1)
        }
    };

    this->_packetReceived = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET), std::bind(
                &pm::PacketManager::parseConnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET), std::bind(
                &pm::PacketManager::parseAcceptationPacket,
                this, std::placeholders::_1),
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET), std::bind(
                &pm::PacketManager::parseDisconnectionPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(EVENT_PACKET), std::bind(
                &pm::PacketManager::parseEventPacket,
                this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(MAP_SEND_PACKET), std::bind(
            &pm::PacketManager::parseMapPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(CAN_START_PACKET), std::bind(
            &pm::PacketManager::parseCanStartPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(SPAWN_PLAYER_PACKET), std::bind(
            &pm::PacketManager::parseSpawnPlayerPacket,
            this, std::placeholders::_1)
        },
        {
            static_cast<uint8_t>(DEATH_PLAYER_PACKET), std::bind(
            &pm::PacketManager::parseDeathPacket,
            this, std::placeholders::_1)
        }
    };

    this->_packetLengths = {
        {
            static_cast<uint8_t>(CONNECTION_CLIENT_PACKET),
            LENGTH_CONNECTION_PACKET
        },
        {
            static_cast<uint8_t>(ACCEPTATION_PACKET),
            LENGTH_ACCEPTATION_PACKET
        },
        {
            static_cast<uint8_t>(DISCONNECTION_PACKET),
            LENGTH_DISCONNECTION_PACKET
        },
        {
            static_cast<uint8_t>(EVENT_PACKET),
            LENGTH_EVENT_PACKET
        },
        {
            static_cast<uint8_t>(DEATH_PLAYER_PACKET),
            LENGTH_DEATH_PACKET
        }
    };

    this->_lengthComb = {
        std::make_tuple(static_cast<uint8_t>(PLAYER_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(TRANSFORM),
        static_cast<uint32_t>(41), static_cast<uint64_t>(6)),
        std::make_tuple(static_cast<uint8_t>(SPEED_COMP),
        static_cast<uint32_t>(9), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(HEALTH),
        static_cast<uint32_t>(17), static_cast<uint64_t>(3)),
        std::make_tuple(static_cast<uint8_t>(COLLIDER),
        static_cast<uint32_t>(34), static_cast<uint64_t>(6)),
        std::make_tuple(static_cast<uint8_t>(SHOOTING_STATS),
        static_cast<uint32_t>(37), static_cast<uint64_t>(6)),
        std::make_tuple(static_cast<uint8_t>(SCORE),
        static_cast<uint32_t>(5), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(AI_MOVEMENT_PATTERN),
        static_cast<uint32_t>(42), static_cast<uint64_t>(7)),
        std::make_tuple(static_cast<uint8_t>(DAMAGE),
        static_cast<uint32_t>(9), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(LIFETIME),
        static_cast<uint32_t>(9), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(VELOCITY),
        static_cast<uint32_t>(17), static_cast<uint64_t>(3)),
        std::make_tuple(static_cast<uint8_t>(AI_MOVER_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(AI_SHOOTER_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(CONTROLLABLE_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(ENEMY_PROJECTILE_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(GAME_ZONE_COLLIDER_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(MOB_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(OBSTACLE_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(PLAYER_PROJECTILE_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(SCORE_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(SHOOTER_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(PROJECTILE_PASS_THROUGH_TAG),
        static_cast<uint32_t>(1), static_cast<uint64_t>(1)),
        std::make_tuple(static_cast<uint8_t>(PROJECTILE_PREFAB),
        static_cast<uint32_t>(0), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(NETWORK_ID),
        static_cast<uint32_t>(9), static_cast<uint64_t>(2)),
        std::make_tuple(static_cast<uint8_t>(GAME_ZONE),
        static_cast<uint32_t>(33), static_cast<uint64_t>(5))
    };

    this->_packGSFunction = {
        std::bind(&pm::PacketManager::packPlayerTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packTransformation,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packSpeed,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packHealth,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packCollider,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packShootingStats,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packScore,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packAIMovementPattern,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packDamage,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packLifetime,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packVelocity,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packAIMoverTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packAIShooterTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packControllableTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packEnemyProjectileTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packGameZoneColliderTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packMobTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packObstacleTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packPlayerProjectileTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packShooterTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packProjectilePassThroughTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packProjectilePrefabComponent,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packNetworkIdComponent,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::packGameZoneComponent,
        this, std::placeholders::_1, std::placeholders::_2)
    };

    this->_unpackGSFunction = {
        std::bind(&pm::PacketManager::unpackPlayerTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackTransform,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackSpeed,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackHealth,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackCollider,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackShootingStats,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackScore,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackAIMovementPattern,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackDamage,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackLifetime,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackVelocity,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackAIMoverTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackAIShooterTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackControllableTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackEnemyProjectileTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackGameZoneColliderTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackMobTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackObstacleTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackPlayerProjectileTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackShooterTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackProjectilePassThroughTag,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackProjectilePrefabComponent,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackNetworkIdComponent,
        this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&pm::PacketManager::unpackGameZoneComponent,
        this, std::placeholders::_1, std::placeholders::_2)
    };
}

pm::PacketManager::~PacketManager() {
    if (this->_serializer) {
        this->_serializer.reset();
    }
}



uint32_t pm::PacketManager::getLength() const {
    return this->_length;
}

uint32_t pm::PacketManager::getSequenceNumber() const {
    return this->_sequenceNumber;
}

uint8_t pm::PacketManager::getType() const {
    return this->_type;
}

std::vector<uint64_t> pm::PacketManager::getPayload() const {
    return this->_payload;
}

uint8_t pm::PacketManager::getIdClient() const {
    return this->_idClient;
}



void pm::PacketManager::setType(uint8_t type) {
    this->_type = type;
}

void pm::PacketManager::setLength(uint32_t length) {
    this->_length = length;
}

void pm::PacketManager::setSequenceNumber(uint32_t sequenceNumber) {
    this->_sequenceNumber = sequenceNumber;
}

void pm::PacketManager::setPayload(std::vector<uint64_t> payload) {
    this->_payload = payload;
}

void pm::PacketManager::setIdClient(uint8_t idClient) {
    this->_idClient = idClient;
}



void pm::PacketManager::reset() {
    this->_idClient = 0;
    this->_sequenceNumber = 0;
    this->_type = NO_OP_PACKET;
    this->_length = 0;
    this->_payload.clear();
}

std::vector<uint64_t> pm::PacketManager::formatString(const std::string str) {
    if (str.size() > 8) {
        throw err::PacketError(
            "[PacketManager] String length exceeds 8 characters",
            err::PacketError::STRING_FORMATTING_ERROR
        );
    }
    int i = 0;
    std::vector<uint64_t> formatted;
    for (char c : str) {
        formatted.push_back(static_cast<uint64_t>(c));
        i++;
    }
    for (; i < 8; i++) {
        formatted.push_back('\0');
    }
    return formatted;
}



extern "C" {

    void *createPacketInstance(unsigned int id) {
        return new pm::PacketManager(id);
    }
    int getType() {
        return PACKET_MODULE;
    }
}
