/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComposantParser
*/

#include "ComposantParser.hpp"
#include <map>
#include <stdexcept>
#include <typeindex>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "../ParserParam.hpp"
#include "../../constants.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../../client/components/rendering/SpriteComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/tags/ControllableTag.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../Error/ParserError.hpp"

ComposantParser::ComposantParser(std::shared_ptr<const std::map<std::string,
    std::pair<std::type_index, std::vector<Field>>>> componentDefinitions,
    const std::map<std::type_index, ComponentCreator>& componentCreators,
    const ShouldParseComponentCallback& shouldParseCallback) :
    _componentDefinitions(componentDefinitions), _componentCreators(componentCreators),
    _shouldParseCallback(shouldParseCallback) {
}

ComposantParser::~ComposantParser() {
}

std::pair<std::shared_ptr<ecs::IComponent>, std::type_index> ComposantParser::parseComponent(
    const std::string& componentName, const nlohmann::json& componentData) {

    if (_componentDefinitions->find(componentName) == _componentDefinitions->end())
        throw err::ParserError("Unknown component: " + componentName,
            err::ParserError::UNKNOWN);

    auto [typeIndex, fieldsDef] = _componentDefinitions->at(componentName);

    std::map<std::string, std::shared_ptr<FieldValue>> fields;

    for (const auto& field : fieldsDef) {
        if (componentData.find(field.name) == componentData.end()) {
            throw err::ParserError("Missing field: " + field.name +
                " in component " + componentName, err::ParserError::MISSING_FIELD);
        }
        fields[field.name] = parseFieldValue(componentData[field.name], field.type);
    }

    if (_shouldParseCallback && !_shouldParseCallback(fields)) {
        return {nullptr, typeIndex};
    }

    auto component = _componentCreators.at(typeIndex)(fields);
    return {component, typeIndex};
}

std::shared_ptr<FieldValue> ComposantParser::parseFieldValue
    (const nlohmann::json& jsonValue, FieldType type) {
    switch (type) {
        case FieldType::VECTOR2F: {
            if (!jsonValue.is_object() ||
                !jsonValue.contains(constants::X_FIELD) ||
                !jsonValue.contains(constants::Y_FIELD)) {
                throw err::ParserError("Invalid Vector2f format",
                    err::ParserError::INVALID_FORMAT);
            }
            math::Vector2f vec(jsonValue[constants::X_FIELD], jsonValue[constants::Y_FIELD]);
            return std::make_shared<FieldValue>(vec);
        }
        case FieldType::FLOAT: {
            if (!jsonValue.is_number()) {
                throw err::ParserError("Invalid float format",
                    err::ParserError::TYPE_MISMATCH);
            }
            return std::make_shared<FieldValue>(jsonValue.get<float>());
        }
        case FieldType::STRING: {
            if (!jsonValue.is_string())
                throw err::ParserError("Invalid string format",
                    err::ParserError::TYPE_MISMATCH);
            return std::make_shared<FieldValue>(jsonValue.get<std::string>());
        }
        case FieldType::INT: {
            if (!jsonValue.is_number_integer())
                throw err::ParserError("Invalid int format",
                    err::ParserError::TYPE_MISMATCH);
            return std::make_shared<FieldValue>(jsonValue.get<int>());
        }
        case FieldType::OBJECT: {
            if (!jsonValue.is_object())
                throw err::ParserError("Invalid object format",
                    err::ParserError::TYPE_MISMATCH);
            std::map<std::string, std::shared_ptr<FieldValue>> objMap;
            for (auto& [key, value] : jsonValue.items()) {
                if (value.is_number_integer()) {
                    objMap[key] = std::make_shared<FieldValue>(value.get<int>());
                } else if (value.is_number_float()) {
                    objMap[key] = std::make_shared<FieldValue>(value.get<float>());
                } else if (value.is_string()) {
                    objMap[key] = std::make_shared<FieldValue>(value.get<std::string>());
                }
            }
            return std::make_shared<FieldValue>(objMap);
        }
        case FieldType::JSON: {
            return std::make_shared<FieldValue>(jsonValue);
        }
        default:
            throw err::ParserError("Unknown field type", err::ParserError::UNKNOWN);
    }
}
