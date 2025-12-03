/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Registry
*/

#include "Registry.hpp"

namespace ecs {

Registry::Registry() : _nextEntityId(1) {
}

Registry::~Registry() {
}

Entity Registry::getMaxEntityId() const {
    Entity maxId = 0;
    for (const auto& pair : _components) {
        Entity componentMaxId = pair.second->getMaxEntityId();
        if (componentMaxId > maxId) {
            maxId = componentMaxId;
        }
    }
    return maxId;
}

Entity Registry::createEntity() {
    return _nextEntityId++;
}

void Registry::destroyEntity(Entity entityId) {
    for (auto& pair : _components) {
        pair.second->remove(entityId);
    }
}

}  // namespace ecs
