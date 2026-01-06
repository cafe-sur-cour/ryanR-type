/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpeedComponent registration
*/

#include "SpeedComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::SpeedComponent,
    constants::SPEEDCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::SPEED_FIELD, FieldType::FLOAT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto speed = std::get<float>(*fields.at(constants::SPEED_FIELD));
        return std::make_shared<ecs::SpeedComponent>(speed);
    }
)
