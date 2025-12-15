/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Delta Tracker - Track and send only modified components
*/

#ifndef COMPONENT_DELTA_TRACKER_HPP
#define COMPONENT_DELTA_TRACKER_HPP

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <map>
#include "../../common/translationToECS.hpp"

namespace rserv {

struct EntitySnapshot {
    uint32_t entityId;
    uint32_t componentMask;  // Bitmask of present components
    std::map<uint8_t, std::vector<uint64_t>> components;

    EntitySnapshot() : entityId(0), componentMask(0) {}
};

class ComponentDeltaTracker {
    public:
        std::vector<uint64_t> createEntityDelta(uint8_t clientId, uint32_t entityId, const EntitySnapshot& currentSnapshot);
        std::vector<uint64_t> createMultiEntityDelta(uint8_t clientId, const std::vector<EntitySnapshot>& entities);
        EntitySnapshot applyDelta(uint8_t clientId, const std::vector<uint64_t>& deltaPayload);
        void clearClientCache(uint8_t clientId);
        void clearEntityCache(uint8_t clientId, uint32_t entityId);
        void clearAllCaches();

    private:
        std::unordered_map<uint8_t, std::unordered_map<uint32_t, EntitySnapshot>> _clientEntityCache;

        std::vector<uint64_t> serializeFullSnapshot(uint32_t entityId, const EntitySnapshot& snapshot);
        std::vector<uint64_t> serializeDelta(uint32_t entityId, uint32_t changedMask, const std::map<uint8_t, std::vector<uint64_t>>& changedComponents);
};

} // namespace rserv

#endif // COMPONENT_DELTA_TRACKER_HPP
