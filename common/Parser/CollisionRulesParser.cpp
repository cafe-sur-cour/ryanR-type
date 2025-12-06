/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CollisionRulesParser
*/

#include "CollisionRulesParser.hpp"
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "../constants.hpp"

namespace ecs {

CollisionRulesData CollisionRulesParser::parseFromFile(const std::string& filePath) {
    std::ifstream collisionFile(filePath);
    if (!collisionFile.is_open()) {
        throw std::runtime_error("Cannot open collision rules file: " + filePath);
    }
    std::string jsonString((std::istreambuf_iterator<char>(collisionFile)),
        std::istreambuf_iterator<char>());
    return parseFromJsonString(jsonString);
}

CollisionRulesData CollisionRulesParser::parseFromJsonString(const std::string& jsonString) {
    CollisionRulesData data;
    nlohmann::json json = nlohmann::json::parse(jsonString);

    if (json.contains(constants::COLLISION_SOLID_KEY)) {
        const auto& solid = json[constants::COLLISION_SOLID_KEY];

        if (solid.contains(constants::COLLISION_ALLOW_KEY)) {
            for (const auto& ruleJson : solid[constants::COLLISION_ALLOW_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.solidAllowRules.push_back(rule);
                }
            }
        }

        if (solid.contains(constants::COLLISION_DENY_KEY)) {
            for (const auto& ruleJson : solid[constants::COLLISION_DENY_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.solidDenyRules.push_back(rule);
                }
            }
        }
    }

    if (json.contains(constants::COLLISION_TRIGGER_KEY)) {
        const auto& trigger = json[constants::COLLISION_TRIGGER_KEY];

        if (trigger.contains(constants::COLLISION_ALLOW_KEY)) {
            for (const auto& ruleJson : trigger[constants::COLLISION_ALLOW_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.triggerAllowRules.push_back(rule);
                }
            }
        }

        if (trigger.contains(constants::COLLISION_DENY_KEY)) {
            for (const auto& ruleJson : trigger[constants::COLLISION_DENY_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.triggerDenyRules.push_back(rule);
                }
            }
        }
    }

    if (json.contains(constants::COLLISION_PUSH_KEY)) {
        const auto& push = json[constants::COLLISION_PUSH_KEY];

        if (push.contains(constants::COLLISION_ALLOW_KEY)) {
            for (const auto& ruleJson : push[constants::COLLISION_ALLOW_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.pushAllowRules.push_back(rule);
                }
            }
        }

        if (push.contains(constants::COLLISION_DENY_KEY)) {
            for (const auto& ruleJson : push[constants::COLLISION_DENY_KEY]) {
                if (ruleJson.size() == 2) {
                    CollisionRule rule;
                    rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                    rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                    data.pushDenyRules.push_back(rule);
                }
            }
        }
    }

    return data;
}

}  // namespace ecs
