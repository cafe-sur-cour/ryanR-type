/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#ifndef GAMERULES_HPP_
#define GAMERULES_HPP_

class GameRules {
    public:
        enum class Difficulty {
            EASY,
            NORMAL,
            HARD
        };

        enum class Gamemode {
            CLASSIC,
            INFINITE
        };

        GameRules();
        ~GameRules() = default;

        void setGamemode(Gamemode gamemode);
        Gamemode getGamemode() const;

        void setDifficulty(Difficulty difficulty);
        Difficulty getDifficulty() const;

        void setCrossfire(bool crossfire);
        bool getCrossfire() const;

    private:
        Gamemode _gamemode;
        Difficulty _difficulty;
        bool _crossfire;
};

#endif /* !GAMERULES_HPP_ */
