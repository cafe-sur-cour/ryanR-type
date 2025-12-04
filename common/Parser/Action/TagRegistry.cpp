/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TagRegistry
*/

#include "TagRegistry.hpp"
#include <memory>
#include <string>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../constants.hpp"

const TagRegistry& TagRegistry::getInstance() {
    static TagRegistry instance;
    return instance;
}

TagRegistry::TagRegistry() {
    initializeTags();
}

bool TagRegistry::hasTag(
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity entity,
    const std::string& tagName
) const {
    auto it = _tagCheckers.find(tagName);
    if (it != _tagCheckers.end()) {
        return it->second(registry, entity);
    }
    return false;
}

void TagRegistry::initializeTags() {
    registerTag<ecs::MobTag>(constants::MOBTAG);
    registerTag<ecs::ProjectileTag>(constants::PROJECTILETAG);
    registerTag<ecs::PlayerTag>(constants::PLAYERTAG);
}
