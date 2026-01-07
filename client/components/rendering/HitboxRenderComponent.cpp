/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HitboxRenderComponent
*/

#include "HitboxRenderComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::HitboxRenderComponent,
    constants::HITBOXRENDERCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING}
    },
    []([[maybe_unused]] const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        return std::make_shared<ecs::HitboxRenderComponent>();
    }
)
