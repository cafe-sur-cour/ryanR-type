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

namespace rserv {
class ComponentSerializer {
    public:
        static std::vector<uint64_t> serializePosition(uint32_t x, uint32_t y);
        static void deserializePosition(const std::vector<uint64_t>& data, uint32_t& x, uint32_t& y);
        static std::vector<uint64_t> serializeVelocity(int32_t vx, int32_t vy);
        static void deserializeVelocity(const std::vector<uint64_t>& data, int32_t& vx, int32_t& vy);
        static std::vector<uint64_t> serializeHealth(uint32_t current, uint32_t max);
        static void deserializeHealth(const std::vector<uint64_t>& data, uint32_t& current, uint32_t& max);
        static std::vector<uint64_t> serializeCollider(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t rotation);
        static std::vector<uint64_t> serializeShootingStats(uint32_t fireRate, uint32_t damage, uint32_t lastShot);
        static std::vector<uint64_t> serializeScore(uint64_t score);
        static std::vector<uint64_t> serializeDamage(uint32_t damage);
        static std::vector<uint64_t> serializeLifetime(uint64_t lifetime);
        static std::vector<uint64_t> serializeSpeed(uint64_t speed);
        static std::vector<uint64_t> serializeAIMovementPattern(uint32_t patternId);
        static std::vector<uint64_t> serializeGameZone(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        static EntitySnapshot createSnapshotFromComponents(uint32_t entityId, const std::vector<uint64_t>& componentData);
        static std::vector<uint64_t> snapshotToComponentData(const EntitySnapshot& snapshot);
        static bool isTagComponent(uint8_t component);
        static bool isOneParamComponent(uint8_t component);
};
} // namespace rserv

#endif // COMPONENT_SERIALIZER_HPP
