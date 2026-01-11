/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#include "GameRules.hpp"

GameRules::GameRules() : _gamemode(CLASSIC), _difficulty(NORMAL), _crossfire(false) {}

void GameRules::setGamemode(Gamemode gamemode) {
    _gamemode = gamemode;
}

Gamemode GameRules::getGamemode() const {
    return _gamemode;
}

void GameRules::setDifficulty(Difficulty difficulty) {
    _difficulty = difficulty;
}

Difficulty GameRules::getDifficulty() const {
    return _difficulty;
}

void GameRules::setCrossfire(bool crossfire) {
    _crossfire = crossfire;
}

bool GameRules::getCrossfire() const {
    return _crossfire;
}
