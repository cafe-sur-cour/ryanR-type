/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingStatsComponent
*/

#ifndef SHOOTINGSTATSCOMPONENT_HPP_
#define SHOOTINGSTATSCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../Prefab/IPrefab.hpp"
#include <memory>
#include <vector>

namespace ecs {

struct MultiShotPattern {
    int shotCount = 1;
    float angleSpread = 0.0f;
    float offsetDistance = 0.0f;
    float angleOffset = 0.0f;
};

class ShootingStatsComponent : public IComponent {
    public:
        ShootingStatsComponent(
            float fireRate = 1.0f,
            const MultiShotPattern &pattern = MultiShotPattern()
        ) : _fireRate(fireRate),
            _multiShotPattern(pattern),
            _cooldownTimer(0.0f) {};
        ~ShootingStatsComponent() = default;

        float getFireRate() const { return _fireRate; };
        void setFireRate(float fireRate) { _fireRate = fireRate; };

        MultiShotPattern getMultiShotPattern() const { return _multiShotPattern; };
        void setMultiShotPattern(const MultiShotPattern &pattern) { _multiShotPattern = pattern; };

        float getCooldownTimer() const { return _cooldownTimer; };
        void setCooldownTimer(float timer) { _cooldownTimer = timer; };

    private:
        float _fireRate;
        MultiShotPattern _multiShotPattern;
        float _cooldownTimer;
};

}  // namespace ecs

#endif /* !SHOOTINGSTATSCOMPONENT_HPP_ */
