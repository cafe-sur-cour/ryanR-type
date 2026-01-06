/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePassThroughTag registration
*/

#include "ProjectilePassThroughTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::ProjectilePassThroughTag, constants::PROJECTILEPASSTHROUGHTAG)
