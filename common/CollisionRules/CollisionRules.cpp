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

CollisionRules::CollisionRules(const CollisionRulesData& data) {
    this->_solidAllowRules = data.solidAllowRules;
    this->_triggerAllowRules = data.triggerAllowRules;
    this->_pushAllowRules = data.pushAllowRules;
}

const std::vector<CollisionRule>& CollisionRules::getAllowRules(
    CollisionType type
) const {
    static const std::vector<CollisionRule> emptyRules;
    switch (type) {
        case CollisionType::Solid:
            return _solidAllowRules ? *_solidAllowRules : emptyRules;
        case CollisionType::Trigger:
            return _triggerAllowRules ? *_triggerAllowRules : emptyRules;
        case CollisionType::Push:
            return _pushAllowRules ? *_pushAllowRules : emptyRules;
        default:
            return emptyRules;
    }
}

bool CollisionRules::canCollide(
    CollisionType type,
    const std::vector<std::string>& tagsA,
    const std::vector<std::string>& tagsB
) const {
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
