/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ObstacleTag registration
*/

#include "ObstacleTag.hpp"
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

REGISTER_TAG_COMPONENT(ecs::ObstacleTag, constants::OBSTACLETAG)
