/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotComponent
*/

#ifndef CHARGEDSHOTCOMPONENT_HPP_
#define CHARGEDSHOTCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include <algorithm>

namespace ecs {

class ChargedShotComponent : public AComponent {
    public:
        ChargedShotComponent(const float &charge = 0, const float &maxCharge = 0) : _charge(charge), _maxCharge(maxCharge) {};
        ~ChargedShotComponent() = default;

        float getCharge() const { return _charge; };
        void setCharge(const float &charge) { _charge = (std::min)(charge, _maxCharge); };

        float getMaxCharge() const { return _maxCharge; };
        void setMaxCharge(const float &maxCharge) { _maxCharge = maxCharge; };

    private:
        float _charge;
        float _maxCharge;
};

}

#endif /* !CHARGEDSHOTCOMPONENT_HPP_ */
