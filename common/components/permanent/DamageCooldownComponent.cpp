/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageCooldownComponent
*/

#include "DamageCooldownComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::DamageCooldownComponent,
    constants::DAMAGECOOLDOWNCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::COOLDOWN_FIELD, FieldType::FLOAT, true,
            std::make_shared<FieldValue>(constants::TRIGGER_DAMAGE_COOLDOWN)}
    },
    static_cast<parser::ComponentCreator>(
        [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        float cooldown = constants::TRIGGER_DAMAGE_COOLDOWN;
        if (fields.find(constants::COOLDOWN_FIELD) != fields.end()) {
            cooldown = std::get<float>(*fields.at(constants::COOLDOWN_FIELD));
        }
        return std::make_shared<ecs::DamageCooldownComponent>(cooldown);
    })
)
