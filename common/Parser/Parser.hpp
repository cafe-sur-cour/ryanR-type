/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "../Prefab/IPrefab.hpp"
#include <memory>
#include "./EntityParser/EntityParser.hpp"
#include "../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "ParserParam.hpp"
#include "./MapParser/MapParser.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"

typedef enum {
    CLIENT = 0,
    SERVER = 1
} ParsingType;

class Parser {
    public:
        Parser(std::shared_ptr<EntityPrefabManager> prefab, ParsingType type,
            std::shared_ptr<ecs::Registry> registry);
        ~Parser();

        std::shared_ptr<EntityPrefabManager> getPrefabManager() const;
        void setPrefabManager(std::shared_ptr<EntityPrefabManager> prefab);
        void parseAllEntities(std::string directoryPath);
        void parseEntity(std::string entityPath);

        const std::map<std::type_index, ComponentAdder>& getComponentAdders() const;
        ParsingType getParsingType() const;
        bool isClientParsing() const;
        bool isServerParsing() const;
        bool shouldParseComponent(std::map<std::string, std::shared_ptr<FieldValue>> fields) const;

        void parseMapFromFile(const std::string& filePath);

        std::shared_ptr<MapParser> getMapParser() const;
        void setRegistry(std::shared_ptr<ecs::Registry> registry);

    private:
        std::shared_ptr<EntityParser> _entityParser;
        std::shared_ptr<MapParser> _mapParser;
        std::shared_ptr<EntityPrefabManager> _prefabManager;

        std::shared_ptr<std::map<std::string, std::pair<std::type_index, std::vector<Field>>>> _componentDefinitions;
        std::map<std::type_index, ComponentCreator> _componentCreators;
        std::map<std::type_index, ComponentAdder> _componentAdders;
        ParsingType _parsingType;
};

#endif /* !PARSER_HPP_ */
