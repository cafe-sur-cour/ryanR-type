/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CollisionRules
*/

#include "CollisionRules.hpp"
#include <algorithm>
#include <string>
#include <vector>

namespace ecs {

const CollisionRules& CollisionRules::getInstance() {
    static CollisionRules instance;
    return instance;
}

void CollisionRules::initWithData(const CollisionRulesData& data) {
    CollisionRules& instance = const_cast<CollisionRules&>(getInstance());
    instance._solidAllowRules = data.solidAllowRules;
    instance._solidDenyRules = data.solidDenyRules;
    instance._triggerAllowRules = data.triggerAllowRules;
    instance._pushAllowRules = data.pushAllowRules;
    instance._pushDenyRules = data.pushDenyRules;
}

CollisionRules::CollisionRules() {
}

const std::vector<CollisionRule>& CollisionRules::getDenyRules(
    CollisionType type
) const {
    switch (type) {
        case CollisionType::Solid:
            return _solidDenyRules;
        case CollisionType::Push:
            return _pushDenyRules;
        default:
            static const std::vector<CollisionRule> emptyRules;
            return emptyRules;
    }
}

const std::vector<CollisionRule>& CollisionRules::getAllowRules(
    CollisionType type
) const {
    switch (type) {
        case CollisionType::Solid:
            return _solidAllowRules;
        case CollisionType::Trigger:
            return _triggerAllowRules;
        case CollisionType::Push:
            return _pushAllowRules;
        default:
            static const std::vector<CollisionRule> emptyRules;
            return emptyRules;
    }
}

bool CollisionRules::canCollide(
    CollisionType type,
    const std::vector<std::string>& tagsA,
    const std::vector<std::string>& tagsB
) const {
    const auto& denyRules = getDenyRules(type);
    for (const auto& rule : denyRules) {
        if (ruleMatches(rule, tagsA, tagsB) || ruleMatches(rule, tagsB, tagsA)) {
            return false;
        }
    }

    const auto& allowRules = getAllowRules(type);
    for (const auto& rule : allowRules) {
        if (ruleMatches(rule, tagsA, tagsB) || ruleMatches(rule, tagsB, tagsA)) {
            return true;
        }
    }

    return false;
}

bool CollisionRules::entityMatchesGroup(
    const std::vector<std::string>& entityTags,
    const std::vector<std::string>& group
) const {
    for (const auto& requiredTag : group) {
        if (std::find(entityTags.begin(), entityTags.end(),
            requiredTag) == entityTags.end()) {
            return false;
        }
    }
    return true;
}

bool CollisionRules::ruleMatches(
    const CollisionRule& rule,
    const std::vector<std::string>& tagsA,
    const std::vector<std::string>& tagsB
) const {
    return entityMatchesGroup(tagsA, rule.groupA) && entityMatchesGroup(tagsB, rule.groupB);
}

}  // namespace ecs
