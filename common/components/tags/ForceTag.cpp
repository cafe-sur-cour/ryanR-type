/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ForceTag
*/

#include "ForceTag.hpp"
#include <string>
#include <map>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::ForceTag,
    constants::FORCE_TAG,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::FORCE_TYPE_FIELD, FieldType::STRING}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto forceType = std::get<std::string>(*fields.at(constants::FORCE_TYPE_FIELD));
        return std::make_shared<ecs::ForceTag>(forceType);
    }
)
