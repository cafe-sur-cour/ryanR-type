/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageIntentComponent
*/

#ifndef DAMAGEINTENTCOMPONENT_HPP_
#define DAMAGEINTENTCOMPONENT_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class DamageIntentComponent : public AComponent {
    public:
        DamageIntentComponent(float damages = 0.0f) : _damages(damages) {};
        ~DamageIntentComponent() = default;

        float getDamages() { return _damages; };
        void setDamages(float damages) { _damages = damages; };

    private:
        float _damages;
};

}  // namespace ecs

#endif /* !DAMAGEINTENTCOMPONENT_HPP_ */
