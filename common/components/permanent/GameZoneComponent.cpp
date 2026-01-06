/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneComponent registration
*/

#include "GameZoneComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"
#include <nlohmann/json.hpp>

REGISTER_COMPONENT(
    ecs::GameZoneComponent,
    constants::GAMEZONECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::ZONERECT_FIELD, FieldType::JSON}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto zoneRectJson = std::get<nlohmann::json>(*fields.at(constants::ZONERECT_FIELD));
        math::FRect zoneRect(zoneRectJson[constants::X_FIELD],
            zoneRectJson[constants::Y_FIELD],
            zoneRectJson[constants::WIDTH_FIELD], 
            zoneRectJson[constants::HEIGHT_FIELD]);
        return std::make_shared<ecs::GameZoneComponent>(zoneRect);
    }
)
