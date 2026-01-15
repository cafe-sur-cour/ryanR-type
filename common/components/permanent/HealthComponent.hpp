/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthComponent
*/

#ifndef HEALTHCOMPONENT_HPP_
#define HEALTHCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../ECS/entity/Entity.hpp"

namespace ecs {

class HealthComponent : public IComponent {
    public:
        HealthComponent(float health = 100) : _health(health), _baseHealth(health), _lastDamageSource(0) {};
        ~HealthComponent() = default;

        float getHealth() const { return _health; }
        void setHealth(float health) { _health = health; };

        float getBaseHealth() const { return _baseHealth; };
        void setBaseHealth(float health) { _baseHealth = health; };

        ecs::Entity getLastDamageSource() const { return _lastDamageSource; }
        void setLastDamageSource(ecs::Entity source) { _lastDamageSource = source; }

    private:
        float _health;
        float _baseHealth;
        ecs::Entity _lastDamageSource;
};

}

#endif /* !HEALTHCOMPONENT_HPP_ */
