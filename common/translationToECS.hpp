/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef TRASLATION_TO_ECS_HPP_
#define TRASLATION_TO_ECS_HPP_

enum componentType {
    TRANSFORM = 0x01,
    VELOCITY = 0x02,
    SPEED = 0x03,
    HEALTH = 0x04,
    COLLIDER = 0x05,
    SHOOTING_STATS = 0x06,
    SCORE = 0x07,
    AI_MOVEMENT_PATTERN = 0x08,
    DAMAGE = 0x09,
    LIFETIME = 0x0A
};

#endif /* !TRASLATION_TO_ECS_HPP_ */
