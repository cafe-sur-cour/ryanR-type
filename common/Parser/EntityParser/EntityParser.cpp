/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityParser
*/

#include "EntityParser.hpp"
#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "../../Error/ParserError.hpp"
#include "../../Prefab/ParsedEntityPrefab.hpp"
#include "../Utils/JsonLoader.hpp"
#include "../Utils/JsonValidation.hpp"

EntityParser::EntityParser(
    std::shared_ptr<const std::map<std::string,
    std::pair<std::type_index, std::vector<Field>>>> componentDefinitions,
    const std::map<std::type_index, ComponentCreator> &componentCreators,
    const std::map<std::type_index, ComponentAdder> &componentAdders,
    const ShouldParseComponentCallback &shouldParseCallback
) :
    _composantParser(componentDefinitions, componentCreators, shouldParseCallback),
    _componentAdders(componentAdders),
    _shouldParseCallback(shouldParseCallback) {
}

EntityParser::~EntityParser() {
}

std::shared_ptr<IPrefab> EntityParser::parseEntity(const std::string &filePath) {
    nlohmann::json jsonData = parser::JsonLoader::loadFromFile(filePath);

    auto validation = parser::JsonValidation::hasRequiredFields(
        jsonData,
        {"name", "components"},
        "Entity file: " + filePath
    );
    if (!validation) {
        std::string errorMsg = "Invalid entity file: " + filePath + "\\n";
        for (const auto& error : validation.errors) {
            errorMsg += "  - " + error + "\\n";
        }
        throw err::ParserError(errorMsg, err::ParserError::INVALID_FORMAT);
    }

    auto prefab = std::make_shared<ParsedEntityPrefab>(jsonData["name"], _componentAdders);

    for (const auto& [componentName, componentData] : jsonData["components"].items()) {
        if (componentData.is_array()) {
            for (const auto& item : componentData) {
                auto [component, typeIndex] =
                    _composantParser.parseComponent(componentName, item);
                if (component != nullptr) {
                    prefab->addComponent(component, typeIndex);
                }
            }
        } else {
            auto [component, typeIndex] =
                _composantParser.parseComponent(componentName, componentData);
            if (component != nullptr) {
                prefab->addComponent(component, typeIndex);
            }
        }
    }

    return prefab;
}
