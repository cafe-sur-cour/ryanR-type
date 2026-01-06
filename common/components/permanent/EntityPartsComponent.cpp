/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPartsComponent registration
*/

#include "EntityPartsComponent.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::EntityPartsComponent, constants::ENTITYPARTSCOMPONENT)
