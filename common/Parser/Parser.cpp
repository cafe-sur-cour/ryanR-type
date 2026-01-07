/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Parser
*/

#include <filesystem>  // NOLINT(build/c++17)
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include "Parser.hpp"
#include "../Prefab/ParsedEntityPrefab.hpp"
#include "../Error/ParserError.hpp"
#include "../constants.hpp"
#include "ComponentRegistry/ComponentRegistry.hpp"

Parser::Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type,
    std::shared_ptr<ecs::Registry> registry) :
    _prefabManager(prefab), _parsingType(type) {
    auto& componentRegistry = parser::ComponentRegistry::getInstance();
    _componentDefinitions = componentRegistry.getComponentDefinitions();
    _componentCreators = componentRegistry.getComponentCreators();
    _componentAdders = componentRegistry.getComponentAdders();

    _mapParser = std::make_shared<MapParser>(_prefabManager, registry);
    auto shouldParseCallback =
        [this](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> bool {
        return this->shouldParseComponent(fields);
    };
    _entityParser = std::make_shared<EntityParser>(_componentDefinitions, _componentCreators,
        _componentAdders, shouldParseCallback);
}

const std::map<std::type_index, ComponentAdder>& Parser::getComponentAdders() const {
    return _componentAdders;
}

Parser::~Parser() {
}

std::shared_ptr<EntityPrefabManager> Parser::getPrefabManager() const {
    return _prefabManager;
}

void Parser::setPrefabManager(std::shared_ptr<EntityPrefabManager> prefabManager) {
    _prefabManager = prefabManager;
}

void Parser::parseAllEntities(std::string directoryPath) {
    for (const auto & entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            parseEntity(entry.path().string());
        }
    }
}

void Parser::parseEntity(std::string entityPath) {
    auto prefab = _entityParser->parseEntity(entityPath);
    std::string name = std::static_pointer_cast<ParsedEntityPrefab>(prefab)->getName();
    _prefabManager->registerPrefab(name, prefab);
}

ParsingType Parser::getParsingType() const {
    return _parsingType;
}

bool Parser::isClientParsing() const {
    return _parsingType == ParsingType::CLIENT;
}

bool Parser::isServerParsing() const {
    return _parsingType == ParsingType::SERVER;
}

bool Parser::shouldParseComponent(std::map<std::string,
    std::shared_ptr<FieldValue>> fields) const {
    auto target = std::get<std::string>(*fields.at(constants::TARGET_FIELD));

    if (target.empty())
        return false;
    if (target == constants::BOTH_VALUE)
        return true;
    if (target == constants::CLIENT_VALUE && isClientParsing())
        return true;
    if (target == constants::SERVER_VALUE && isServerParsing())
        return true;
    return false;
}

void Parser::parseMapFromFile(const std::string& filePath) {
    _mapParser->parseMapFromFile(filePath);
}

void Parser::parseMapFromJson(const nlohmann::json& mapJson) {
    _mapParser->parseMap(mapJson);
}

std::shared_ptr<MapParser> Parser::getMapParser() const {
    return _mapParser;
}

void Parser::setRegistry(std::shared_ptr<ecs::Registry> registry) {
    nlohmann::json mapData = _mapParser->getMapJson();
    _mapParser = std::make_shared<MapParser>(_prefabManager, registry);
    if (!mapData.is_null()) {
        _mapParser->setMapJson(mapData);
    }
}
