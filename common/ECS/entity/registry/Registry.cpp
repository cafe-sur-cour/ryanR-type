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
#include "../../../components/permanent/NetworkIdComponent.hpp"

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

Entity Registry::getEntityByNetworkId(size_t networkId) {
    auto view = this->view<ecs::NetworkIdComponent>();
    for (auto entity : view) {
        auto netIdComp = getComponent<ecs::NetworkIdComponent>(entity);
        if (netIdComp && netIdComp->getNetworkId() == networkId) {
            return entity;
        }
    }
    return 0;
}

void Registry::setOnEntityDestroyed(std::function<void(Entity)> callback) {
    _onEntityDestroyed = callback;
}

}  // namespace ecs
