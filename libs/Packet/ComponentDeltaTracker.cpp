/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Component Delta Tracker
*/

#include "ComponentDeltaTracker.hpp"

std::vector<uint64_t> pm::ComponentDeltaTracker::createEntityDelta(uint8_t clientId,
    uint32_t entityId, const EntitySnapshot& currentSnapshot) {
    std::vector<uint64_t> delta;
    auto clientIt = _clientEntityCache.find(clientId);
    bool isFirstSend = (clientIt == _clientEntityCache.end());

    if (isFirstSend) {
        _clientEntityCache[clientId][entityId] = currentSnapshot;
        return serializeFullSnapshot(entityId, currentSnapshot);
    }

    auto& entityCache = clientIt->second;
    auto entityIt = entityCache.find(entityId);
    if (entityIt == entityCache.end()) {
        entityCache[entityId] = currentSnapshot;
        return serializeFullSnapshot(entityId, currentSnapshot);
    }

    const EntitySnapshot& lastSnapshot = entityIt->second;
    uint32_t changedMask = 0;
    std::map<uint8_t, std::vector<uint64_t>> changedComponents;

    for (const auto& [compType, compData] : currentSnapshot.components) {
        auto lastIt = lastSnapshot.components.find(compType);
        if (lastIt == lastSnapshot.components.end() || lastIt->second != compData) {
            changedMask |= (1u << compType);
            changedComponents[compType] = compData;
        }
    }

    for (const auto& [compType, _] : lastSnapshot.components) {
        if (currentSnapshot.components.find(compType) == currentSnapshot.components.end()) {
            changedMask |= (1u << compType);
            changedComponents[compType] = {};
        }
    }

    if (changedMask == 0) {
        return {};
    }
    entityCache[entityId] = currentSnapshot;
    return serializeDelta(entityId, changedMask, changedComponents);
}

std::vector<uint64_t> pm::ComponentDeltaTracker::createMultiEntityDelta(uint8_t clientId,
    const std::vector<EntitySnapshot>& entities) {
    std::vector<uint64_t> payload;
    std::vector<std::vector<uint64_t>> entityDeltas;

    for (const auto& entity : entities) {
        auto delta = createEntityDelta(clientId, entity.entityId, entity);
        if (!delta.empty()) {
            entityDeltas.push_back(delta);
        }
    }

    payload.push_back(entityDeltas.size());
    for (const auto& delta : entityDeltas) {
        payload.insert(payload.end(), delta.begin(), delta.end());
    }
    return payload;
}

pm::EntitySnapshot pm::ComponentDeltaTracker::applyDelta(uint8_t clientId,
    const std::vector<uint64_t>& deltaPayload) {
    if (deltaPayload.size() < 2) {
        return {};
    }

    size_t offset = 0;
    uint32_t entityId = static_cast<uint32_t>(deltaPayload[offset++]);
    uint32_t componentMask = static_cast<uint32_t>(deltaPayload[offset++]);
    EntitySnapshot snapshot;
    snapshot.entityId = entityId;
    snapshot.componentMask = componentMask;

    auto clientIt = _clientEntityCache.find(clientId);
    if (clientIt != _clientEntityCache.end()) {
        auto entityIt = clientIt->second.find(entityId);
        if (entityIt != clientIt->second.end()) {
            snapshot = entityIt->second;
        }
    }

    for (uint8_t compType = 0; compType < 32 && offset < deltaPayload.size(); compType++) {
        if (componentMask & (1u << compType)) {
            if (offset >= deltaPayload.size()) break;
            size_t compSize = static_cast<size_t>(deltaPayload[offset++]);
            if (compSize == 0) {
                snapshot.components.erase(compType);
                snapshot.componentMask &= ~(1u << compType);
            } else {
                std::vector<uint64_t> compData;
                for (size_t j = 0; j < compSize && offset < deltaPayload.size(); j++) {
                    compData.push_back(deltaPayload[offset++]);
                }
                snapshot.components[compType] = compData;
                snapshot.componentMask |= (1u << compType);
            }
        }
    }

    _clientEntityCache[clientId][entityId] = snapshot;
    return snapshot;
}

void pm::ComponentDeltaTracker::clearClientCache(uint8_t clientId) {
    _clientEntityCache.erase(clientId);
}

void pm::ComponentDeltaTracker::clearEntityCache(uint8_t clientId, uint32_t entityId) {
    auto it = _clientEntityCache.find(clientId);
    if (it != _clientEntityCache.end()) {
        it->second.erase(entityId);
    }
}

void pm::ComponentDeltaTracker::clearAllCaches() {
    _clientEntityCache.clear();
}

std::vector<uint64_t> pm::ComponentDeltaTracker::serializeFullSnapshot(uint32_t entityId,
    const EntitySnapshot& snapshot) {
    std::vector<uint64_t> payload;

    payload.push_back(entityId);
    payload.push_back(snapshot.componentMask);
    for (uint8_t compType = 0; compType < 32; compType++) {
        if (snapshot.componentMask & (1u << compType)) {
            auto it = snapshot.components.find(compType);
            if (it != snapshot.components.end()) {
                payload.push_back(it->second.size());
                payload.insert(payload.end(), it->second.begin(), it->second.end());
            }
        }
    }
    return payload;
}

std::vector<uint64_t> pm::ComponentDeltaTracker::serializeDelta(uint32_t entityId, uint32_t changedMask,
    const std::map<uint8_t, std::vector<uint64_t>>& changedComponents) {
    std::vector<uint64_t> payload;

    payload.push_back(entityId);
    payload.push_back(changedMask);
    for (uint8_t compType = 0; compType < 32; compType++) {
        if (changedMask & (1u << compType)) {
            auto it = changedComponents.find(compType);
            if (it != changedComponents.end()) {
                payload.push_back(it->second.size());
                payload.insert(payload.end(), it->second.begin(), it->second.end());
            }
        }
    }
    return payload;
}
