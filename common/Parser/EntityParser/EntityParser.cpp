/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityParser
*/

#include "EntityParser.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "../../Prefab/ParsedEntityPrefab.hpp"

EntityParser::EntityParser(const std::map<std::string, std::pair<std::type_index, std::vector<Field>>>& componentDefinitions, const std::map<std::type_index, ComponentCreator>& componentCreators, const std::map<std::type_index, ComponentAdder>& componentAdders) : _composantParser(componentDefinitions, componentCreators), _componentAdders(componentAdders) {
}

EntityParser::~EntityParser() {
}

std::shared_ptr<IPrefab> EntityParser::parseEntity(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    nlohmann::json jsonData;
    file >> jsonData;

    auto prefab = std::make_shared<ParsedEntityPrefab>(jsonData["name"], _componentAdders);

    for (const auto& [componentName, componentData] : jsonData["components"].items()) {
        auto [component, typeIndex] = _composantParser.parseComponent(componentName, componentData);
        prefab->addComponent(component, typeIndex);
    }

    return prefab;
}
