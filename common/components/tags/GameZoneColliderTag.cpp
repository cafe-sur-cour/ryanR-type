/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneColliderTag registration
*/

#include "GameZoneColliderTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::GameZoneColliderTag, constants::GAMEZONECOLLIDERTAG)
