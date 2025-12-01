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

class Parser {
    public:
        Parser(std::shared_ptr<EntityPrefabManager> prefab);
        ~Parser();

        std::shared_ptr<EntityPrefabManager> getPrefabManager() const;
        void setPrefabManager(std::shared_ptr<EntityPrefabManager> prefab);
        void parseAllEntities(std::string directoryPath);
        void getNamegetNamegetNamegetNamegetName(std::string entityPath);
        void parseEntity(std::string entityPath);

        void instanciateComponentDefinitions();
        void instanciateComponentCreators();
        void instanciateComponentAdders();
        const std::map<std::type_index, ComponentAdder>& getComponentAdders() const;

    private:
        std::shared_ptr<EntityParser> _entityParser;
        std::shared_ptr<EntityPrefabManager> _prefabManager;

        std::map<std::string, std::pair<std::type_index, std::vector<Field>>> _componentDefinitions;
        std::map<std::type_index, ComponentCreator> _componentCreators;
        std::map<std::type_index, ComponentAdder> _componentAdders;
};

#endif /* !PARSER_HPP_ */
