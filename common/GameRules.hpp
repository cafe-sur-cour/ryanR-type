/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameRules
*/

#ifndef GAMERULES_HPP_
#define GAMERULES_HPP_

enum Difficulty {
    EASY,
    NORMAL,
    HARD
};

class GameRules {
    public:
        GameRules();
        ~GameRules() = default;

        void setDifficulty(Difficulty difficulty);
        Difficulty getDifficulty() const;

    private:
        Difficulty _difficulty;
};

#endif /* !GAMERULES_HPP_ */
