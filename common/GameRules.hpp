/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#ifndef GAMERULES_HPP_
#define GAMERULES_HPP_

namespace GameRulesNS {

enum Difficulty {
    EASY,
    NORMAL,
    HARD
};

enum Gamemode {
    CLASSIC,
    INFINITE
};

}  // namespace GameRulesNS

class GameRules {
    public:
        GameRules();
        ~GameRules() = default;

        void setGamemode(GameRulesNS::Gamemode gamemode);
        GameRulesNS::Gamemode getGamemode() const;

        void setDifficulty(GameRulesNS::Difficulty difficulty);
        GameRulesNS::Difficulty getDifficulty() const;

        void setCrossfire(bool crossfire);
        bool getCrossfire() const;

    private:
        GameRulesNS::Gamemode _gamemode;
        GameRulesNS::Difficulty _difficulty;
        bool _crossfire;
};

#endif /* !GAMERULES_HPP_ */
