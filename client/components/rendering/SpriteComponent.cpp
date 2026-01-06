/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpriteComponent registration
*/

#include "SpriteComponent.hpp"
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::SpriteComponent,
    constants::SPRITECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::FILEPATH_FIELD, FieldType::STRING}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto path = std::get<std::string>(*fields.at(constants::FILEPATH_FIELD));
        return std::make_shared<ecs::SpriteComponent>(path);
    }
)
