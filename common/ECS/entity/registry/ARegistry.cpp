/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ARegistry
*/

#include "ARegistry.hpp"

namespace ecs {

ARegistry::ARegistry() {
}

ARegistry::~ARegistry() {
}

void ARegistry::removeAllComponentsWithState(ComponentState state) {
    for (const auto& pair : _components) {
        pair.second->removeAllComponentsWithState(state);
    }
}

size_t ARegistry::getMaxEntityId() const {
    size_t maxId = 0;
    for (const auto& pair : _components) {
        size_t componentMaxId = pair.second->getMaxEntityId();
        if (componentMaxId > maxId) {
            maxId = componentMaxId;
        }
    }
    return maxId;
}

}  // namespace ecs
