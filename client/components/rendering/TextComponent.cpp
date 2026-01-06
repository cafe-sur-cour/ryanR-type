/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextComponent registration
*/

#include "TextComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::TextComponent,
    constants::TEXTCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::TEXT_FIELD, FieldType::STRING},
        Field{constants::FONTPATH_FIELD, FieldType::STRING},
        Field{constants::COLOR_FIELD, FieldType::OBJECT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto text = std::get<std::string>(*fields.at(constants::TEXT_FIELD));
        auto fontPath = std::get<std::string>(*fields.at(constants::FONTPATH_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::TextComponent>(text, fontPath, color);
    }
)
