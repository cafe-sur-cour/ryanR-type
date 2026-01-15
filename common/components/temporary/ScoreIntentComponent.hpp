/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreIntentComponent
*/

#ifndef SCOREINTENTCOMPONENT_HPP_
#define SCOREINTENTCOMPONENT_HPP_

class ScoreIntentComponent {
    public:
        ScoreIntentComponent(int score = 0) : _score(score) {};
        ~ScoreIntentComponent() = default;

        int getScore() const { return _score; }
        void setScore(int newScore) { _score = newScore; }
    protected:
    private:
        int _score;
};

#endif /* !SCOREINTENTCOMPONENT_HPP_ */
