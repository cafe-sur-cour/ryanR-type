/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShooterTag registration
*/

#include "ShooterTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::ShooterTag, constants::SHOOTERTAG)
