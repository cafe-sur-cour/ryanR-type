/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EnnemyProjectileTag registration
*/

#include "EnnemyProjectileTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::EnnemyProjectileTag, constants::ENNEMYPROJECTILETAG)
