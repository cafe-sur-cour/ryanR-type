/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerProjectileTag registration
*/

#include "PlayerProjectileTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::PlayerProjectileTag, constants::PLAYERPROJECTILETAG)
