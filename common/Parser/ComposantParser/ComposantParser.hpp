/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComposantParser
*/

#ifndef COMPOSANTPARSER_HPP_
#define COMPOSANTPARSER_HPP_

#include <string>
#include <memory>
#include <map>
#include <typeindex>
#include "../../components/base/IComponent.hpp"
#include "../ParserParam.hpp"
#include <nlohmann/json.hpp>
#include <functional>

class ComposantParser {
    public:
        using ShouldParseComponentCallback = std::function<bool(const std::map<std::string, std::shared_ptr<FieldValue>>&)>;

        ComposantParser(
            std::shared_ptr<const std::map<std::string, std::pair<std::type_index, std::vector<Field>>>> componentDefinitions,
            const std::map<std::type_index, ComponentCreator> &componentCreators,
            const ShouldParseComponentCallback &shouldParseCallback = nullptr
        );
        ~ComposantParser();

        std::pair<std::shared_ptr<ecs::IComponent>, std::type_index> parseComponent(const std::string &componentName, const nlohmann::json &componentData);

    protected:
    private:
        std::shared_ptr<FieldValue> parseFieldValue(const nlohmann::json &jsonValue, FieldType type);
        std::shared_ptr<const std::map<std::string, std::pair<std::type_index, std::vector<Field>>>> _componentDefinitions;
        const std::map<std::type_index, ComponentCreator> &_componentCreators;
        ShouldParseComponentCallback _shouldParseCallback;
};

#endif /* !COMPOSANTPARSER_HPP_ */
