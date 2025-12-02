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

Parser::Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type) :
    _prefabManager(prefab), _parsingType(type) {
    instanciateComponentDefinitions();
    instanciateComponentCreators();
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
    for (const auto & entry : std::filesystem::directory_iterator(directoryPath)) {
        parseEntity(entry.path().string());
    }
}

void Parser::parseEntity(std::string entityPath) {
    std::cout << "Parsing entity from: " << entityPath << std::endl;
    try {
        auto prefab = _entityParser->parseEntity(entityPath);
        std::string name = std::static_pointer_cast<ParsedEntityPrefab>(prefab)->getName();
        std::cout << "Registering prefab: " << name << std::endl;
        _prefabManager->registerPrefab(name, prefab);
    } catch (const err::ParserError& e) {
        throw err::ParserError(std::string("Failed to parse entity '") +
        entityPath + "': " + e.what());
    }
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
    auto target = std::get<std::string>(*fields.at("target"));

    if (target.empty())
        return false;
    if (target == "both")
        return true;
    if (target == "client" && isClientParsing())
        return true;
    if (target == "server" && isServerParsing())
        return true;
    return false;
}
