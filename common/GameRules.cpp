/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#include "GameRules.hpp"

GameRules::GameRules() : _difficulty(NORMAL) {}

void GameRules::setDifficulty(Difficulty difficulty) {
    _difficulty = difficulty;
}

Difficulty GameRules::getDifficulty() const {
    return _difficulty;
}
