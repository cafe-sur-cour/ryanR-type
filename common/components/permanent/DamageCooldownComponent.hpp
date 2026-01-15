/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageCooldownComponent
*/

#ifndef DAMAGECOOLDOWNCOMPONENT_HPP_
#define DAMAGECOOLDOWNCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../constants.hpp"

namespace ecs {

class DamageCooldownComponent : public IComponent {
    public:
        DamageCooldownComponent(float cooldown = constants::TRIGGER_DAMAGE_COOLDOWN)
            : _cooldown(cooldown), _lastDamageTime(0.0f) {};
        ~DamageCooldownComponent() = default;

        float getCooldown() const { return _cooldown; }
        void setCooldown(float cooldown) { _cooldown = cooldown; }

        float getLastDamageTime() const { return _lastDamageTime; }
        void setLastDamageTime(float time) { _lastDamageTime = time; }

    private:
        float _cooldown;
        float _lastDamageTime;
};

}

#endif /* !DAMAGECOOLDOWNCOMPONENT_HPP_ */
