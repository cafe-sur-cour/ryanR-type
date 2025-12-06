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
#include <nlohmann/json.hpp>

namespace ecs {

const CollisionRules& CollisionRules::getInstance() {
    static CollisionRules instance;
    return instance;
}

CollisionRules::CollisionRules() {
    std::ifstream collisionFile("configs/collision_rules.json");
    if (collisionFile.is_open()) {
        std::string jsonString((std::istreambuf_iterator<char>(collisionFile)),
            std::istreambuf_iterator<char>());
        try {
            loadFromJson(jsonString);
        } catch (const std::exception& e) {
        }
    }
}

void CollisionRules::loadFromJson(const std::string& jsonString) {
    nlohmann::json json = nlohmann::json::parse(jsonString);

    _solidAllowRules.clear();
    _solidDenyRules.clear();
    _triggerAllowRules.clear();
    _pushAllowRules.clear();
    _pushDenyRules.clear();

    if (json.contains("solid")) {
        const auto& solid = json["solid"];

        if (solid.contains("allow")) {
            for (const auto& ruleJson : solid["allow"]) {
                if (ruleJson.size() == 2) {
                    Rule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    _solidAllowRules.push_back(rule);
                }
            }
        }

        if (solid.contains("deny")) {
            for (const auto& ruleJson : solid["deny"]) {
                if (ruleJson.size() == 2) {
                    Rule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    _solidDenyRules.push_back(rule);
                }
            }
        }
    }

    if (json.contains("trigger")) {
        const auto& trigger = json["trigger"];

        if (trigger.contains("allow")) {
            for (const auto& ruleJson : trigger["allow"]) {
                if (ruleJson.size() == 2) {
                    Rule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    _triggerAllowRules.push_back(rule);
                }
            }
        }
    }

    if (json.contains("push")) {
        const auto& push = json["push"];

        if (push.contains("allow")) {
            for (const auto& ruleJson : push["allow"]) {
                if (ruleJson.size() == 2) {
                    Rule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    _pushAllowRules.push_back(rule);
                }
            }
        }

        if (push.contains("deny")) {
            for (const auto& ruleJson : push["deny"]) {
                if (ruleJson.size() == 2) {
                    Rule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    _pushDenyRules.push_back(rule);
                }
            }
        }
    }
}

bool CollisionRules::canCollide(
    CollisionType type,
    const std::vector<std::string>& tagsA,
    const std::vector<std::string>& tagsB
) const {
    const std::vector<Rule>* denyRules = nullptr;
    if (type == CollisionType::Solid) {
        denyRules = &_solidDenyRules;
    } else if (type == CollisionType::Push) {
        denyRules = &_pushDenyRules;
    }

    if (denyRules) {
        for (const auto& rule : *denyRules) {
            if (ruleMatches(rule, tagsA, tagsB) || ruleMatches(rule, tagsB, tagsA)) {
                return false;
            }
        }
    }

    const std::vector<Rule>* allowRules = nullptr;
    if (type == CollisionType::Solid) {
        allowRules = &_solidAllowRules;
    } else if (type == CollisionType::Trigger) {
        allowRules = &_triggerAllowRules;
    } else if (type == CollisionType::Push) {
        allowRules = &_pushAllowRules;
    }

    if (allowRules) {
        for (const auto& rule : *allowRules) {
            if (ruleMatches(rule, tagsA, tagsB) || ruleMatches(rule, tagsB, tagsA)) {
                return true;
            }
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
    const Rule& rule,
    const std::vector<std::string>& tagsA,
    const std::vector<std::string>& tagsB
) const {
    return entityMatchesGroup(tagsA, rule.groupA) && entityMatchesGroup(tagsB, rule.groupB);
}

}  // namespace ecs
