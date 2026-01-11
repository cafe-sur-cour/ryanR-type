/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#include "GameRules.hpp"

GameRules::GameRules() :
    _gamemode(GameRules::Gamemode::CLASSIC),
    _difficulty(GameRules::Difficulty::NORMAL),
    _crossfire(false) {}

void GameRules::setGamemode(GameRules::Gamemode gamemode) {
    _gamemode = gamemode;
}

GameRules::Gamemode GameRules::getGamemode() const {
    return _gamemode;
}

void GameRules::setDifficulty(GameRules::Difficulty difficulty) {
    _difficulty = difficulty;
}

GameRules::Difficulty GameRules::getDifficulty() const {
    return _difficulty;
}

void GameRules::setCrossfire(bool crossfire) {
    _crossfire = crossfire;
}

bool GameRules::getCrossfire() const {
    return _crossfire;
}
