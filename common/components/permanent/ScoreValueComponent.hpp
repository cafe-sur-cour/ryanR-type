/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreValueComponent
*/

#ifndef SCOREVALUECOMPONENT_HPP_
#define SCOREVALUECOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ScoreValueComponent : public IComponent {
    public:
        ScoreValueComponent(int scoreValue = 0) : _scoreValue(scoreValue) {};
        ~ScoreValueComponent() override = default;

        int getScoreValue() const { return _scoreValue; }
        void setScoreValue(int scoreValue) { _scoreValue = scoreValue; }

    private:
        int _scoreValue;
};

}  // namespace ecs

#endif /* !SCOREVALUECOMPONENT_HPP_ */