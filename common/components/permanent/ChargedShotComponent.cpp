/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotComponent
*/

#include "ChargedShotComponent.hpp"
#include <memory>
#include <map>
#include <string>
#include "../../constants.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"

REGISTER_COMPONENT(
    ecs::ChargedShotComponent,
    constants::CHARGEDSHOTCOMPONENT,
    {
        Field(constants::TARGET_FIELD, FieldType::STRING),
        Field(constants::MAXCHARGE_FIELD, FieldType::FLOAT)
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto maxCharge = std::get<float>(*fields.at(constants::MAXCHARGE_FIELD));
        return std::make_shared<ecs::ChargedShotComponent>(0.0f, maxCharge);
    }
)
