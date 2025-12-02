/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootIntentComponent
*/

#ifndef SHOOTINTENTCOMPONENT_HPP_
#define SHOOTINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class ShootIntentComponent : public AComponent {
    public:
        ShootIntentComponent(const math::Vector2f &position = math::Vector2f(0.0f, 0.0f))
            : _position(position) {
        };
        ~ShootIntentComponent() = default;

        math::Vector2f getPosition() const { return _position; };
        void setPosition(const math::Vector2f &position) { _position = position; };

    private:
        math::Vector2f _position;
};

}  // namespace ecs

#endif /* !SHOOTINTENTCOMPONENT_HPP_ */
