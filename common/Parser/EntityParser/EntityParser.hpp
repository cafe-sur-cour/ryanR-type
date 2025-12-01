/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityParser
*/

#ifndef ENTITYPARSER_HPP_
#define ENTITYPARSER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "../../components/base/IComponent.hpp"
#include "../ParserParam.hpp"
#include "../ComposantParser/ComposantParser.hpp"
#include "../../Prefab/IPrefab.hpp"
#include "../../Prefab/ParsedEntityPrefab.hpp"
#include <nlohmann/json.hpp>

class EntityParser {
    public:
        EntityParser(const std::map<std::string, std::pair<std::type_index, std::vector<Field>>>& componentDefinitions, const std::map<std::type_index, ComponentCreator>& componentCreators, const std::map<std::type_index, ComponentAdder>& componentAdders);
        ~EntityParser();

        std::shared_ptr<IPrefab> parseEntity(const std::string& filePath);

    protected:
    private:
        ComposantParser _composantParser;
        const std::map<std::type_index, ComponentAdder>& _componentAdders;
};

#endif /* !ENTITYPARSER_HPP_ */
