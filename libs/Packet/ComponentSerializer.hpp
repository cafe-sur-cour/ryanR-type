/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Serializer - Helper for converting ECS components to/from packets
*/

#ifndef COMPONENT_SERIALIZER_HPP
#define COMPONENT_SERIALIZER_HPP

#include "ComponentDeltaTracker.hpp"
#include <cstring>
#include <string>

namespace pm {

class ComponentSerializer {
    public:
        std::vector<uint64_t> serializePosition(uint32_t x, uint32_t y);
        void deserializePosition(const std::vector<uint64_t>& data, uint32_t& x, uint32_t& y);
        std::vector<uint64_t> serializeVelocity(int32_t vx, int32_t vy);
        void deserializeVelocity(const std::vector<uint64_t>& data, int32_t& vx, int32_t& vy);
        std::vector<uint64_t> serializeHealth(uint32_t current, uint32_t max);
        void deserializeHealth(const std::vector<uint64_t>& data, uint32_t& current, uint32_t& max);
        std::vector<uint64_t> serializeCollider(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t rotation);
        std::vector<uint64_t> serializeShootingStats(uint32_t fireRate, uint32_t damage, uint32_t lastShot);
        std::vector<uint64_t> serializeScore(uint64_t score);
        std::vector<uint64_t> serializeDamage(uint32_t damage);
        std::vector<uint64_t> serializeLifetime(uint64_t lifetime);
        std::vector<uint64_t> serializeSpeed(uint64_t speed);
        std::vector<uint64_t> serializeAIMovementPattern(uint32_t patternId);
        std::vector<uint64_t> serializeNetworkId(uint32_t networkId);
        std::vector<uint64_t> serializeGameZone(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        EntitySnapshot createSnapshotFromComponents(uint32_t entityId, const std::vector<uint64_t>& componentData);
        std::vector<uint64_t> snapshotToComponentData(const EntitySnapshot& snapshot);
};

} // namespace pm

#endif // COMPONENT_SERIALIZER_HPP
