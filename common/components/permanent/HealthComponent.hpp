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
        HealthComponent(float health = 100) : _health(health), _baseHealth(health) {};
        ~HealthComponent() override = default;

        float getHealth() const { return _health; }
        void setHealth(float health) { _health = health; };

        void decreaseHealth(float quantity) { _health -= quantity; };

        float getBaseHealth() const { return _baseHealth; };
        void setBaseHealth(float health) { _baseHealth = health; };

    private:
        float _health;
        float _baseHealth;
};

}

#endif /* !HEALTHCOMPONENT_HPP_ */
