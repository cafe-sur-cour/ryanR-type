/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParserParam
*/

#ifndef PARSERPARAM_HPP_
#define PARSERPARAM_HPP_

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <functional>
#include <memory>
#include "../types/Vector2f.hpp"
#include <nlohmann/json.hpp>
#include "../components/base/IComponent.hpp"
#include "../components/permanent/TransformComponent.hpp"
#include "../components/permanent/VelocityComponent.hpp"
#include "../components/permanent/SpeedComponent.hpp"
#include "../../client/components/rendering/SpriteComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../components/tags/ControllableTag.hpp"
#include "../components/tags/PlayerTag.hpp"
#include "../components/permanent/ColliderComponent.hpp"

enum class ParserParam {
    NONE,
    NAME,
    COMPONENTS,
};

enum class FieldType {
    VECTOR2F,
    FLOAT,
    STRING,
    INT,
    BOOL,
    OBJECT,
    JSON,
};

struct Field {
    std::string name;
    FieldType type;
};

struct FieldValue;

using FieldValueMap = std::map<std::string, std::shared_ptr<FieldValue>>;
using FieldValueVariant = std::variant<math::Vector2f, float, std::string, int, bool, FieldValueMap, nlohmann::json>;

struct FieldValue : FieldValueVariant {
    using FieldValueVariant::FieldValueVariant;
    using FieldValueVariant::operator=;
};

#include <typeindex>
#include "../ECS/entity/registry/Registry.hpp"
#include "../ECS/entity/Entity.hpp"

using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(const std::map<std::string, std::shared_ptr<FieldValue>>&)>;
using ComponentAdder = std::function<void(std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>)>;

#endif /* !PARSERPARAM_HPP_ */
