/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#include "GameRules.hpp"

GameRules::GameRules() :
    _gamemode(GameRulesNS::Gamemode::CLASSIC),
    _difficulty(GameRulesNS::Difficulty::NORMAL),
    _crossfire(false) {}

void GameRules::setGamemode(GameRulesNS::Gamemode gamemode) {
    _gamemode = gamemode;
}

GameRulesNS::Gamemode GameRules::getGamemode() const {
    return _gamemode;
}

void GameRules::setDifficulty(GameRulesNS::Difficulty difficulty) {
    _difficulty = difficulty;
}

GameRulesNS::Difficulty GameRules::getDifficulty() const {
    return _difficulty;
}

void GameRules::setCrossfire(bool crossfire) {
    _crossfire = crossfire;
}

bool GameRules::getCrossfire() const {
    return _crossfire;
}
