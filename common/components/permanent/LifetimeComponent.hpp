/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LifetimeComponent
*/

#ifndef LIFETIMECOMPONENT_HPP_
#define LIFETIMECOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class LifetimeComponent : public IComponent {
    public:
        LifetimeComponent(float lifetime = 0.0f) : _lifetime(lifetime) {};
        ~LifetimeComponent() = default;

        float getLifetime() const { return _lifetime; };
        void setLifetime(float lifetime) { _lifetime = lifetime; };

    private:
        float _lifetime;
};

} // ecs


#endif /* !LIFETIMECOMPONENT_HPP_ */
