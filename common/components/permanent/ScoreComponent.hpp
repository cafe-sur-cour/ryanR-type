/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreComponent
*/

#ifndef SCORECOMPONENT_HPP_
#define SCORECOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ScoreComponent : public IComponent {
    public:
        ScoreComponent(int score = 0) : _score(score) {};
        ~ScoreComponent() {};

        int getScore() const { return _score; }
        void setScore(int score) { _score = score; }

    private:
        int _score;
};

}  // namespace ecs

#endif /* !SCORECOMPONENT_HPP_ */
