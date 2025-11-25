/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** VelocityComponent
*/

#ifndef VELOCITYCOMPONENT_HPP_
#define VELOCITYCOMPONENT_HPP_

#include "AComponent.hpp"
#include "../../types/Vector2f.hpp"

namespace ecs {

class VelocityComponent : public AComponent {
    public:
        VelocityComponent(math::Vector2f velocity = math::Vector2f(0.0f, 0.0f)) : _velocity(velocity) {};
        ~VelocityComponent() = default;

        math::Vector2f getVelocity() const { return _velocity; };
        void setVelocity(math::Vector2f velocity) { _velocity = velocity; };
    private:
        math::Vector2f _velocity;
};

} // namespace ecs

#endif /* !VELOCITYCOMPONENT_HPP_ */
