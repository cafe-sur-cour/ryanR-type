/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpeedComponent
*/

#ifndef SPEEDCOMPONENT_HPP_
#define SPEEDCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "constants.hpp"

namespace ecs {

class SpeedComponent : public IComponent {
    public:
        SpeedComponent(float speed = constants::BASE_SPEED) : _speed(speed) {};
        ~SpeedComponent() = default;

        float getSpeed() const { return _speed; };
        void setSpeed(float speed) { _speed = speed; };

    private:
        float _speed;
};

}  // namespace ecs

#endif /* !SPEEDCOMPONENT_HPP_ */
