/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** VelocityComponent
*/

#include "VelocityComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_COMPONENT(
    ecs::VelocityComponent,
    constants::VELOCITYCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING}
    },
    []([[maybe_unused]] const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        return std::make_shared<ecs::VelocityComponent>();
    }
)
