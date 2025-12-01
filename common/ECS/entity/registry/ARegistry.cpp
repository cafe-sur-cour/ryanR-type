/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ARegistry
*/

#include "ARegistry.hpp"

namespace ecs {

ARegistry::ARegistry() : _nextEntityId(1) {
}

ARegistry::~ARegistry() {
}

void ARegistry::removeAllComponentsWithState(ComponentState state) {
    for (const auto& pair : _components) {
        pair.second->removeAllComponentsWithState(state);
    }
}

Entity ARegistry::getMaxEntityId() const {
    Entity maxId = 0;
    for (const auto& pair : _components) {
        Entity componentMaxId = pair.second->getMaxEntityId();
        if (componentMaxId > maxId) {
            maxId = componentMaxId;
        }
    }
    return maxId;
}

Entity ARegistry::createEntity() {
    return _nextEntityId++;
}

}  // namespace ecs
