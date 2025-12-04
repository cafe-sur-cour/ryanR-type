/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TagRegistry
*/

#include "TagRegistry.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

TagRegistry& TagRegistry::getInstance() {
    static TagRegistry instance;
    return instance;
}

bool TagRegistry::hasTag(std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, const std::string& tagName) const {
    auto it = _tagCheckers.find(tagName);
    if (it != _tagCheckers.end()) {
        return it->second(registry, entity);
    }
    return false;
}