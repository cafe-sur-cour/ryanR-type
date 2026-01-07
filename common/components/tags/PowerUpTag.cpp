/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** PowerUpTag
*/

#include "PowerUpTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::PowerUpTag, constants::POWERUP_TAG)
