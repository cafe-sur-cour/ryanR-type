/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LifetimeComponent registration
*/

#include "LifetimeComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::LifetimeComponent,
    constants::LIFETIMECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::LIFETIME_FIELD, FieldType::FLOAT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        float lifetime = 0.0f;
        if (fields.find(constants::LIFETIME_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFETIME_FIELD));
        } else if (fields.find(constants::LIFESPAN_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFESPAN_FIELD));
        }
        return std::make_shared<ecs::LifetimeComponent>(lifetime);
    }
)

REGISTER_COMPONENT(
    ecs::LifetimeComponent,
    constants::LIFESPANCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::LIFESPAN_FIELD, FieldType::FLOAT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        float lifetime = 0.0f;
        if (fields.find(constants::LIFETIME_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFETIME_FIELD));
        } else if (fields.find(constants::LIFESPAN_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFESPAN_FIELD));
        }
        return std::make_shared<ecs::LifetimeComponent>(lifetime);
    }
)
