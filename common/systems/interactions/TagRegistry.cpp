/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TagRegistry
*/

#include "TagRegistry.hpp"
#include <memory>
#include <string>
#include <vector>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../components/tags/PlayerProjectileTag.hpp"
#include "../../components/tags/EnnemyProjectileTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/tags/ControllableTag.hpp"
#include "../../components/tags/GameZoneColliderTag.hpp"
#include "../../components/tags/ObstacleTag.hpp"
#include "../../components/tags/ProjectilePassThroughTag.hpp"
#include "../../components/tags/ShooterTag.hpp"
#include "../../../client/components/tags/BackGroundMusicTag.hpp"
#include "../../components/tags/PowerUpTag.hpp"
#include "../../constants.hpp"

const TagRegistry& TagRegistry::getInstance() {
    static TagRegistry instance;
    return instance;
}

TagRegistry::TagRegistry() {
    this->_tagCheckers = {};
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

std::vector<std::string> TagRegistry::getTags(
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity entity
) const {
    std::vector<std::string> tags;
    for (const auto& pair : _tagCheckers) {
        if (pair.second(registry, entity)) {
            tags.push_back(pair.first);
        }
    }
    return tags;
}

void TagRegistry::initializeTags() {
    registerTag<ecs::MobTag>(constants::MOBTAG);
    registerTag<ecs::PlayerProjectileTag>(constants::PLAYERPROJECTILETAG);
    registerTag<ecs::EnnemyProjectileTag>(constants::ENNEMYPROJECTILETAG);
    registerTag<ecs::PlayerTag>(constants::PLAYERTAG);
    registerTag<ecs::ControllableTag>(constants::CONTROLLABLETAG);
    registerTag<ecs::GameZoneColliderTag>(constants::GAMEZONECOLLIDERTAG);
    registerTag<ecs::ObstacleTag>(constants::OBSTACLETAG);
    registerTag<ecs::ProjectilePassThroughTag>(constants::PROJECTILEPASSTHROUGHTAG);
    registerTag<ecs::ShooterTag>(constants::SHOOTERTAG);
    registerTag<ecs::BackGroundMusicTag>(constants::BACKGROUNDMUSICTAG);
    registerTag<ecs::PowerUpTag>(constants::POWERUP_TAG);
}
