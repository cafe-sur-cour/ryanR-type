/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Serializer
*/

#include "ComponentSerializer.hpp"

std::vector<uint64_t> rserv::ComponentSerializer::serializePosition(uint32_t x, uint32_t y) {
    uint64_t packed = (static_cast<uint64_t>(x) << 32) | y;
    return {packed};
}

void rserv::ComponentSerializer::deserializePosition(const std::vector<uint64_t>& data,
    uint32_t& x, uint32_t& y) {
    if (!data.empty()) {
        x = static_cast<uint32_t>(data[0] >> 32);
        y = static_cast<uint32_t>(data[0] & 0xFFFFFFFF);
    }
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeVelocity(int32_t vx, int32_t vy) {
    uint64_t packed = (static_cast<uint64_t>(static_cast<uint32_t>(vx)) << 32) |
        static_cast<uint32_t>(vy);
    return {packed};
}

void rserv::ComponentSerializer::deserializeVelocity(const std::vector<uint64_t>& data,
    int32_t& vx, int32_t& vy) {
    if (!data.empty()) {
        vx = static_cast<int32_t>(data[0] >> 32);
        vy = static_cast<int32_t>(data[0] & 0xFFFFFFFF);
    }
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeHealth(uint32_t current,
    uint32_t max) {
    uint64_t packed = (static_cast<uint64_t>(current) << 32) | max;
    return {packed};
}

void rserv::ComponentSerializer::deserializeHealth(const std::vector<uint64_t>& data,
    uint32_t& current, uint32_t& max) {
    if (!data.empty()) {
        current = static_cast<uint32_t>(data[0] >> 32);
        max = static_cast<uint32_t>(data[0] & 0xFFFFFFFF);
    }
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeCollider(uint32_t x, uint32_t y,
    uint32_t width, uint32_t height, uint32_t rotation) {
    uint64_t pos = (static_cast<uint64_t>(x) << 32) | y;
    uint64_t size = (static_cast<uint64_t>(width) << 32) | height;
    return {pos, size, rotation};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeShootingStats(uint32_t fireRate,
    uint32_t damage, uint32_t lastShot) {
    uint64_t packed1 = (static_cast<uint64_t>(fireRate) << 32) | damage;
    return {packed1, lastShot};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeScore(uint64_t score) {
    return {score};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeDamage(uint32_t damage) {
    return {damage};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeLifetime(uint64_t lifetime) {
    return {lifetime};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeSpeed(uint64_t speed) {
    return {speed};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeAIMovementPattern(
    uint32_t patternId) {
    return {patternId};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeNetworkId(uint32_t networkId) {
    return {networkId};
}

std::vector<uint64_t> rserv::ComponentSerializer::serializeGameZone(uint32_t x, uint32_t y,
    uint32_t width, uint32_t height) {
    uint64_t pos = (static_cast<uint64_t>(x) << 32) | y;
    uint64_t size = (static_cast<uint64_t>(width) << 32) | height;
    return {pos, size};
}

rserv::EntitySnapshot rserv::ComponentSerializer::createSnapshotFromComponents(
    uint32_t entityId, const std::vector<uint64_t>& componentData) {
    EntitySnapshot snapshot;
    snapshot.entityId = entityId;
    snapshot.componentMask = 0;

    size_t i = 0;
    while (i < componentData.size()) {
        uint8_t compType = static_cast<uint8_t>(componentData[i]);
        if (rserv::ComponentSerializer::isTagComponent(compType)) {
            snapshot.componentMask |= (1u << compType);
            snapshot.components[compType] = {};
            i++;
            continue;
        }

        if (rserv::ComponentSerializer::isOneParamComponent(compType)) {
            if (i + 1 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {componentData[i + 1]};
                i += 2;
            } else {
                i++;
            }
            continue;
        }

        if (compType == TRANSFORM || compType == COLLIDER) {
            if (i + 5 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2],
                    componentData[i + 3], componentData[i + 4],
                    componentData[i + 5]
                };
                i += 6;
            } else {
                i++;
            }
            continue;
        }

        if (compType == HEALTH || compType == VELOCITY) {
            if (i + 2 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2]
                };
                i += 3;
            } else {
                i++;
            }
            continue;
        }

        if (compType == SHOOTING_STATS) {
            if (i + 3 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2],
                    componentData[i + 3]
                };
                i += 4;
            } else {
                i++;
            }
            continue;
        }

        if (compType == GAME_ZONE) {
            if (i + 4 < componentData.size()) {
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = {
                    componentData[i + 1], componentData[i + 2],
                    componentData[i + 3], componentData[i + 4]
                };
                i += 5;
            } else {
                i++;
            }
            continue;
        }

        if (compType == PROJECTILE_PREFAB) {
            if (i + 1 < componentData.size()) {
                std::vector<uint64_t> prefabData;
                prefabData.reserve(32);
                size_t j = i + 1;
                while (j < componentData.size()) {
                    prefabData.push_back(componentData[j]);
                    if (j + 2 < componentData.size() &&
                        componentData[j] == static_cast<uint64_t>('\r') &&
                        componentData[j + 1] == static_cast<uint64_t>('\n') &&
                        componentData[j + 2] == static_cast<uint64_t>('\0')) {
                        j += 3;
                        break;
                    }
                    j++;
                }
                snapshot.componentMask |= (1u << compType);
                snapshot.components[compType] = std::move(prefabData);
                i = j;
            } else {
                i++;
            }
            continue;
        }
        i++;
    }

    return snapshot;
}

std::vector<uint64_t> rserv::ComponentSerializer::snapshotToComponentData(
    const EntitySnapshot& snapshot) {
    std::vector<uint64_t> data;

    for (const auto& [compType, compData] : snapshot.components) {
        data.push_back(compType);
        data.insert(data.end(), compData.begin(), compData.end());
    }
    return data;
}

bool rserv::ComponentSerializer::isTagComponent(uint8_t component) {
    if (component == PLAYER_TAG || (component >= AI_MOVER_TAG &&
        component <= PROJECTILE_PASS_THROUGH_TAG)) {
        return true;
    }
    return false;
}

bool rserv::ComponentSerializer::isOneParamComponent(uint8_t component) {
    if (component == SPEED_COMP || component == DAMAGE || component == LIFETIME ||
        component == SCORE || component == AI_MOVEMENT_PATTERN || component == NETWORK_ID) {
        return true;
    }
    return false;
}
