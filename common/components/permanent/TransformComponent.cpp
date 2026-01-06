/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TransformComponent registration
*/

#include "TransformComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::TransformComponent,
    constants::TRANSFORMCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::POSITION_FIELD, FieldType::VECTOR2F},
        Field{constants::SCALE_FIELD, FieldType::VECTOR2F, true, 
              std::make_shared<FieldValue>(math::Vector2f(1.0f, 1.0f))},
        Field{constants::ROTATION_FIELD, FieldType::FLOAT, true, 
              std::make_shared<FieldValue>(0.0f)}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto pos = std::get<math::Vector2f>(*fields.at(constants::POSITION_FIELD));
        auto scale = std::get<math::Vector2f>(*fields.at(constants::SCALE_FIELD));
        auto rot = std::get<float>(*fields.at(constants::ROTATION_FIELD));
        return std::make_shared<ecs::TransformComponent>(pos, rot, scale);
    }
)
