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
    NONE = 0,
    NAME = 1,
    COMPONENTS = 2,
};

enum class FieldType {
    VECTOR2F = 0,
    FLOAT = 1,
    STRING = 2,
    INT = 3,
    BOOL = 4,
    OBJECT = 5,
    JSON = 6,
    UNDEFINED = 7
};

using FieldValueMap = std::map<std::string, std::shared_ptr<struct FieldValue>>;
using FieldValueVariant = std::variant<math::Vector2f, float, std::string, int, bool, FieldValueMap, nlohmann::json>;

struct FieldValue : FieldValueVariant {
    using FieldValueVariant::FieldValueVariant;
    using FieldValueVariant::operator=;

    template<typename T>
    FieldValue(T&& value) : FieldValueVariant(std::forward<T>(value)) {}
};

struct Field {
    std::string name = "";
    FieldType type;
    bool optional = false;
    std::shared_ptr<FieldValue> defaultValue = nullptr;

    Field(std::string n, FieldType t, bool opt = false, std::shared_ptr<FieldValue> def = nullptr)
        : name(std::move(n)), type(t), optional(opt), defaultValue(std::move(def)) {}
};

#include <typeindex>
#include "../ECS/entity/registry/Registry.hpp"
#include "../ECS/entity/Entity.hpp"

using ComponentCreator = std::function<std::shared_ptr<ecs::IComponent>(const std::map<std::string, std::shared_ptr<FieldValue>>&)>;
using ComponentAdder = std::function<void(std::shared_ptr<ecs::Registry>, ecs::Entity, std::shared_ptr<ecs::IComponent>)>;

#endif /* !PARSERPARAM_HPP_ */
