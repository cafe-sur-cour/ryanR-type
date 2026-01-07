/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerTag
*/

#include "PlayerTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::PlayerTag, constants::PLAYERTAG)
