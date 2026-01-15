/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputIntentComponent
*/

#ifndef INPUTINTENTCOMPONENT_HPP_
#define INPUTINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class InputIntentComponent : public IComponent {
    public:
        InputIntentComponent(const math::Vector2f &direction = math::Vector2f(0.0f, 0.0f))
            : _direction(direction) {
        };
        ~InputIntentComponent(){
            _direction = math::Vector2f(0.0f, 0.0f);
        };

        math::Vector2f getDirection() const { return _direction; };
        void setDirection(const math::Vector2f &direction) { _direction = direction; };

    private:
        math::Vector2f _direction;
};

}  // namespace ecs

#endif /* !INPUTINTENTCOMPONENT_HPP_ */
