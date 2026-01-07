/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CollisionRulesParser
*/

#include "CollisionRulesParser.hpp"
#include <string>
#include <map>
#include <memory>
#include <utility>
#include <nlohmann/json.hpp>
#include "../constants.hpp"
#include "../Error/ClientError.hpp"
#include "../Error/ParserError.hpp"
#include "Utils/JsonLoader.hpp"

namespace ecs {

CollisionRulesData CollisionRulesParser::parseFromFile(const std::string& filePath) {
    try {
        nlohmann::json json = parser::JsonLoader::loadFromFile(filePath);
        return parseFromJson(json);
    } catch (const err::ParserError&) {
        throw err::ClientError("Cannot open collision rules file: " +
            filePath, err::ClientError::CAN_NOT_OPEN_FILE);
    }
}

void CollisionRulesParser::parseRulesForType(
    const nlohmann::json& typeJson,
    std::shared_ptr<std::vector<CollisionRule>> allowRules
) {
    if (typeJson.contains(constants::COLLISION_ALLOW_KEY)) {
        for (const auto& ruleJson : typeJson[constants::COLLISION_ALLOW_KEY]) {
            if (ruleJson.size() == 2) {
                CollisionRule rule;
                rule.groupA = ruleJson[0].get<std::vector<std::string>>();
                rule.groupB = ruleJson[1].get<std::vector<std::string>>();
                allowRules->push_back(rule);
            }
        }
    }
}

CollisionRulesData CollisionRulesParser::parseFromJson(const nlohmann::json& json) {
    CollisionRulesData data;
    data.solidAllowRules = std::make_shared<std::vector<CollisionRule>>();
    data.triggerAllowRules = std::make_shared<std::vector<CollisionRule>>();
    data.pushAllowRules = std::make_shared<std::vector<CollisionRule>>();

    const std::map<std::string, std::shared_ptr<std::vector<CollisionRule>>> typeMap = {
        {constants::COLLISION_SOLID_KEY, data.solidAllowRules},
        {constants::COLLISION_TRIGGER_KEY, data.triggerAllowRules},
        {constants::COLLISION_PUSH_KEY, data.pushAllowRules}
    };

    for (const auto& [typeKey, allowRules] : typeMap) {
        if (json.contains(typeKey)) {
            const auto& typeJson = json[typeKey];
            parseRulesForType(typeJson, allowRules);
        }
    }

    return data;
}

CollisionRulesData CollisionRulesParser::parseFromJsonString(const std::string& jsonString) {
    nlohmann::json json = nlohmann::json::parse(jsonString);
    return parseFromJson(json);
}

}  // namespace ecs
