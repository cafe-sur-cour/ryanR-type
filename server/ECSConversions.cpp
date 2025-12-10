/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Conversions
*/

#include "Server.hpp"
#include "../common/translationToECS.hpp"

std::vector<uint64_t> convertPlayerEntity() {
    std::vector<uint64_t> data;
    data.push_back(static_cast<uint64_t>(TRANSFORM));
    data.push_back(static_cast<uint64_t>(SPEED));
    data.push_back(static_cast<uint64_t>(HEALTH));
    data.push_back(static_cast<uint64_t>(COLLIDER));
    data.push_back(static_cast<uint64_t>(SHOOTING_STATS));
    data.push_back(static_cast<uint64_t>(SCORE));
    return data;
}

std::vector<uint64_t> convertEnemyEntity() {
    std::vector<uint64_t> data;
    data.push_back(static_cast<uint64_t>(TRANSFORM));
    data.push_back(static_cast<uint64_t>(SPEED));
    data.push_back(static_cast<uint64_t>(HEALTH));
    data.push_back(static_cast<uint64_t>(COLLIDER));
    data.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));
    data.push_back(static_cast<uint64_t>(DAMAGE));
    return data;
}

std::vector<uint64_t> convertProjectileEntity() {
    std::vector<uint64_t> data;
    data.push_back(static_cast<uint64_t>(TRANSFORM));
    data.push_back(static_cast<uint64_t>(SPEED));
    data.push_back(static_cast<uint64_t>(DAMAGE));
    data.push_back(static_cast<uint64_t>(LIFETIME));
    return data;
}

std::vector<uint64_t> convertObstacleBreakableEntity() {
    std::vector<uint64_t> data;
    data.push_back(static_cast<uint64_t>(TRANSFORM));
    data.push_back(static_cast<uint64_t>(COLLIDER));
    return data;
}

std::vector<uint64_t> convertObstacleUnbreakableEntity() {
    std::vector<uint64_t> data;
    data.push_back(static_cast<uint64_t>(TRANSFORM));
    data.push_back(static_cast<uint64_t>(SPEED));
    return data;
}