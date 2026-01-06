/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageComponent registration
*/

#include "DamageComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::DamageComponent,
    constants::DAMAGECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::DAMAGE_FIELD, FieldType::FLOAT}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto damage = std::get<float>(*fields.at(constants::DAMAGE_FIELD));
        return std::make_shared<ecs::DamageComponent>(damage);
    }
)
