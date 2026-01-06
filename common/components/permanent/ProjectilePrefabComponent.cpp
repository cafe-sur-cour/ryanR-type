/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePrefabComponent
*/

#include "ProjectilePrefabComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::ProjectilePrefabComponent,
    constants::PROJECTILEPREFABCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::PREFABNAME_FIELD, FieldType::STRING}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto prefabName = std::get<std::string>(*fields.at(constants::PREFABNAME_FIELD));
        return std::make_shared<ecs::ProjectilePrefabComponent>(prefabName);
    }
)
