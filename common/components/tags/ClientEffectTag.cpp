/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientEffectTag registration
*/

#include "ClientEffectTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::ClientEffectTag, constants::CLIENTEFFECTTAG)
