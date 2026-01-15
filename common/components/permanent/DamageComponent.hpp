/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageComponent
*/

#ifndef DAMAGECOMPONENT_HPP_
#define DAMAGECOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class DamageComponent : public IComponent {
    public:
        DamageComponent(float damage = 0.0f) : _damage(damage) {};
        ~DamageComponent() = default;

        float getDamage() const { return _damage; }
        void setDamage(float damage) { _damage = damage; }

    private:
        float _damage;
};

}  // namespace ecs

#endif /* !DAMAGECOMPONENT_HPP_ */