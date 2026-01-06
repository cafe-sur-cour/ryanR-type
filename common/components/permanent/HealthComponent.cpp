/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthComponent
*/

#include "HealthComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::HealthComponent,
    constants::HEALTHCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::HEALTH_FIELD, FieldType::FLOAT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto health = std::get<float>(*fields.at(constants::HEALTH_FIELD));
        return std::make_shared<ecs::HealthComponent>(health);
    }
)
