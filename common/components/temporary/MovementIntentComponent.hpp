/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MovementIntentComponent
*/

#ifndef MOVEMENTINTENTCOMPONENT_HPP_
#define MOVEMENTINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class MovementIntentComponent : public IComponent {
    public:
        MovementIntentComponent(const math::Vector2f &direction = math::Vector2f(0.0f, 0.0f), bool active = false)
            : _direction(direction), _active(active) {
        };
        ~MovementIntentComponent() = default;

        math::Vector2f getDirection() const { return _direction; };
        void setDirection(const math::Vector2f &direction) { _direction = direction; };

        bool isActive() const { return _active; };
        void setActive(bool active) { _active = active; };

    private:
        math::Vector2f _direction;
        bool _active;
};

}  // namespace ecs

#endif /* !MOVEMENTINTENTCOMPONENT_HPP_ */
