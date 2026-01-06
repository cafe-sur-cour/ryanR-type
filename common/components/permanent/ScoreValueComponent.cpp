/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreValueComponent registration
*/

#include "ScoreValueComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::ScoreValueComponent,
    constants::SCOREVALUECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::SCOREVALUE_FIELD, FieldType::INT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto scoreValue = std::get<int>(*fields.at(constants::SCOREVALUE_FIELD));
        return std::make_shared<ecs::ScoreValueComponent>(scoreValue);
    }
)
