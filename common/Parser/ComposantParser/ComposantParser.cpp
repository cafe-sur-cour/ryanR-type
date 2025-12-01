/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComposantParser
*/

#include "ComposantParser.hpp"
#include "../ParserParam.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../../client/components/rendering/SpriteComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/tags/ControllableTag.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include <stdexcept>
#include <typeindex>
#include <map>

ComposantParser::ComposantParser(const std::map<std::string, std::pair<std::type_index, std::vector<Field>>>& componentDefinitions, const std::map<std::type_index, ComponentCreator>& componentCreators) : _componentDefinitions(componentDefinitions), _componentCreators(componentCreators) {
}

ComposantParser::~ComposantParser() {
}

std::pair<std::shared_ptr<ecs::IComponent>, std::type_index> ComposantParser::parseComponent(const std::string& componentName, const nlohmann::json& componentData) {

    if (_componentDefinitions.find(componentName) == _componentDefinitions.end()) {
        throw std::runtime_error("Unknown component: " + componentName);
    }

    auto [typeIndex, fieldsDef] = _componentDefinitions.at(componentName);

    std::map<std::string, std::shared_ptr<FieldValue>> fields;

    for (const auto& field : fieldsDef) {
        if (componentData.find(field.name) == componentData.end()) {
            throw std::runtime_error("Missing field: " + field.name + " in component " + componentName);
        }
        fields[field.name] = parseFieldValue(componentData[field.name], field.type);
    }

    auto component = _componentCreators.at(typeIndex)(fields);
    return {component, typeIndex};
}

std::shared_ptr<FieldValue> ComposantParser::parseFieldValue(const nlohmann::json& jsonValue, FieldType type) {
    switch (type) {
        case FieldType::VECTOR2F: {
            if (!jsonValue.is_object() || !jsonValue.contains("x") || !jsonValue.contains("y")) {
                throw std::runtime_error("Invalid Vector2f format");
            }
            math::Vector2f vec(jsonValue["x"], jsonValue["y"]);
            return std::make_shared<FieldValue>(vec);
        }
        case FieldType::FLOAT: {
            if (!jsonValue.is_number()) {
                throw std::runtime_error("Invalid float format");
            }
            return std::make_shared<FieldValue>(jsonValue.get<float>());
        }
        case FieldType::STRING: {
            if (!jsonValue.is_string()) {
                throw std::runtime_error("Invalid string format");
            }
            return std::make_shared<FieldValue>(jsonValue.get<std::string>());
        }
        case FieldType::INT: {
            if (!jsonValue.is_number_integer()) {
                throw std::runtime_error("Invalid int format");
            }
            return std::make_shared<FieldValue>(jsonValue.get<int>());
        }
        default:
            throw std::runtime_error("Unknown field type");
    }
}
