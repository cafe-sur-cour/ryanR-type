/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RectangleRenderComponent
*/

#include "RectangleRenderComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::RectangleRenderComponent,
    constants::RECTANGLERENDERCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::WIDTH_FIELD, FieldType::FLOAT},
        Field{constants::HEIGHT_FIELD, FieldType::FLOAT},
        Field{constants::COLOR_FIELD, FieldType::OBJECT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto width = std::get<float>(*fields.at(constants::WIDTH_FIELD));
        auto height = std::get<float>(*fields.at(constants::HEIGHT_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::RectangleRenderComponent>(color, width, height);
    }
)
