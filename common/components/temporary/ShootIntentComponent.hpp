/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootIntentComponent
*/

#ifndef SHOOTINTENTCOMPONENT_HPP_
#define SHOOTINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class ShootIntentComponent : public IComponent {
    public:
        ShootIntentComponent(float angle = 0.0f) : _angle(angle) {
            _position = math::Vector2f(0.0f, 0.0f);
        }
        ~ShootIntentComponent() = default;

        void setAngle(float angle) { _angle = angle; }
        float getAngle() const { return _angle; }

    private:
        float _angle;
        math::Vector2f _position;
};

}  // namespace ecs

#endif /* !SHOOTINTENTCOMPONENT_HPP_ */
