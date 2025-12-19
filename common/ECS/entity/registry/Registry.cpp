/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Registry
*/

#include <string>
#include <memory>
#include <unordered_map>

#include "Registry.hpp"

namespace ecs {

Registry::Registry() : _nextEntityId(1), _onEntityDestroyed(nullptr) {
    this->_components = std::unordered_map<std::string, std::shared_ptr<IComponentArray>>();
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
    if (_onEntityDestroyed) {
        _onEntityDestroyed(entityId);
    }
    for (auto& pair : _components) {
        pair.second->removeComponents(entityId);
    }
}

void Registry::setOnEntityDestroyed(std::function<void(Entity)> callback) {
    _onEntityDestroyed = callback;
}

}  // namespace ecs
