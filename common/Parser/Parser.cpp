/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Parser
*/

#include "Parser.hpp"
#include <filesystem>
#include <iostream>
#include "../Prefab/ParsedEntityPrefab.hpp"

Parser::Parser(std::shared_ptr<EntityPrefabManager> prefab) : _prefabManager(prefab) {
    instanciateComponentDefinitions();
    instanciateComponentCreators();
    instanciateComponentAdders();
    _entityParser = std::make_shared<EntityParser>(_componentDefinitions, _componentCreators, _componentAdders);
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
    auto prefab = _entityParser->parseEntity(entityPath);
    std::string name = std::static_pointer_cast<ParsedEntityPrefab>(prefab)->getName();
    std::cout << "Registering prefab: " << name << std::endl;
    _prefabManager->registerPrefab(name, prefab);
}