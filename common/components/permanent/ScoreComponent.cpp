/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreComponent registration
*/

#include "ScoreComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::ScoreComponent,
    constants::SCORECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::SCORE_FIELD, FieldType::INT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto score = std::get<int>(*fields.at(constants::SCORE_FIELD));
        return std::make_shared<ecs::ScoreComponent>(score);
    }
)
