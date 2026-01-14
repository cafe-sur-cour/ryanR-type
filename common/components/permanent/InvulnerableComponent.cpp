/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InvulnerableComponent
*/

#include "InvulnerableComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::InvulnerableComponent,
    constants::INVULNERABLECOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::ACTIVE_FIELD, FieldType::BOOL, true,
            std::make_shared<FieldValue>(false)}
    },
    static_cast<parser::ComponentCreator>(
        [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        bool active = false;
        if (fields.find(constants::ACTIVE_FIELD) != fields.end()) {
            active = std::get<bool>(*fields.at(constants::ACTIVE_FIELD));
        }
        return std::make_shared<ecs::InvulnerableComponent>(active);
    })
)
