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

}  // namespace ecs
