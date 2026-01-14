/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthComponent
*/

#ifndef HEALTHCOMPONENT_HPP_
#define HEALTHCOMPONENT_HPP_

#include <iostream>
#include "../base/AComponent.hpp"
#include "../../ECS/entity/Entity.hpp"

namespace ecs {

class HealthComponent : public AComponent {
    public:
        HealthComponent(float health = 100) : _health(health), _baseHealth(health), _lastDamageSource(0), _invulnerable(false) {};
        ~HealthComponent() override = default;

        float getHealth() const { return _health; }
        void setHealth(float health) { _health = health; };

        void decreaseHealth(float quantity) { if (!_invulnerable) _health -= quantity; };

        float getBaseHealth() const { return _baseHealth; };
        void setBaseHealth(float health) { _baseHealth = health; };

        ecs::Entity getLastDamageSource() const { return _lastDamageSource; }
        void setLastDamageSource(ecs::Entity source) { _lastDamageSource = source; }

        void increaseHealth(float quantity) { _health += quantity; };

        bool isInvulnerable() const { return _invulnerable; }
        void setInvulnerable(bool invulnerable) { _invulnerable = invulnerable; }

    private:
        float _health;
        float _baseHealth;
        ecs::Entity _lastDamageSource;
        bool _invulnerable;
};

}

#endif /* !HEALTHCOMPONENT_HPP_ */
