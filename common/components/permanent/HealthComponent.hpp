/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthComponent
*/

#ifndef HEALTHCOMPONENT_HPP_
#define HEALTHCOMPONENT_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class HealthComponent : public AComponent {
    public:
        explicit HealthComponent(float health = 100) : _health(health) {};
        ~HealthComponent() override = default;

        float getHealth() const { return _health; }
        void setHealth(float health) { _health = health; }

    private:
        float _health;
};

}

#endif /* !HEALTHCOMPONENT_HPP_ */
