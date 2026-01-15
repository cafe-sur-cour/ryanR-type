/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotComponent
*/

#ifndef CHARGEDSHOTCOMPONENT_HPP_
#define CHARGEDSHOTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <algorithm>

namespace ecs {

class ChargedShotComponent : public IComponent {
    public:
        ChargedShotComponent(
            const float &charge = 0,
            const float &maxCharge = 0,
            const float &chargeReloadTime = 0
        ) : _charge(charge), _maxCharge(maxCharge), _chargeReloadTime(chargeReloadTime) {};
        ~ChargedShotComponent() = default;

        float getCharge() const { return _charge; };
        void setCharge(const float &charge) { _charge = charge; };

        float getMaxCharge() const { return _maxCharge; };
        void setMaxCharge(const float &maxCharge) { _maxCharge = maxCharge; };

        float getReloadTime() const { return _chargeReloadTime; };
        void setReloadTime(const float &reloadTime) { _chargeReloadTime = reloadTime; };

    private:
        float _charge;
        float _maxCharge;
        float _chargeReloadTime;
};

}

#endif /* !CHARGEDSHOTCOMPONENT_HPP_ */
