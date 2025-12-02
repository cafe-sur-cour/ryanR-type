/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingStatsComponent
*/

#ifndef SHOOTINGSTATSCOMPONENT_HPP_
#define SHOOTINGSTATSCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../Prefab/IPrefab.hpp"
#include <memory>
#include <vector>

namespace ecs {

struct MultiShotPattern {
    int shotCount = 1;
    float angleSpread = 0.0f;
    float offsetDistance = 0.0f;

    MultiShotPattern() = default;
    MultiShotPattern(int count, float spread = 0.0f, float offset = 0.0f)
        : shotCount(count), angleSpread(spread), offsetDistance(offset) {}
};

class ShootingStatsComponent : public AComponent {
    public:
        ShootingStatsComponent(
            float fireRate = 1.0f,
            std::shared_ptr<IPrefab> projectilePrefab = nullptr,
            float projectileSpeed = 300.0f,
            const MultiShotPattern &pattern = MultiShotPattern()
        ) : _fireRate(fireRate),
            _projectilePrefab(projectilePrefab),
            _projectileSpeed(projectileSpeed),
            _multiShotPattern(pattern),
            _cooldownTimer(0.0f) {};
        ~ShootingStatsComponent() = default;

        float getFireRate() const { return _fireRate; };
        void setFireRate(float fireRate) { _fireRate = fireRate; };

        std::shared_ptr<IPrefab> getProjectilePrefab() const { return _projectilePrefab; };
        void setProjectilePrefab(std::shared_ptr<IPrefab> prefab) { _projectilePrefab = prefab; };

        float getProjectileSpeed() const { return _projectileSpeed; };
        void setProjectileSpeed(float speed) { _projectileSpeed = speed; };

        MultiShotPattern getMultiShotPattern() const { return _multiShotPattern; };
        void setMultiShotPattern(const MultiShotPattern &pattern) { _multiShotPattern = pattern; };

        float getCooldownTimer() const { return _cooldownTimer; };
        void setCooldownTimer(float timer) { _cooldownTimer = timer; };

        bool canShoot() const { return _cooldownTimer <= 0.0f; };

        void updateCooldown(float deltaTime) {
            if (_cooldownTimer > 0.0f) {
                _cooldownTimer -= deltaTime;
            }
        };

        void resetCooldown() {
            _cooldownTimer = 1.0f / _fireRate;
        };

    private:
        float _fireRate;
        std::shared_ptr<IPrefab> _projectilePrefab;
        float _projectileSpeed;
        MultiShotPattern _multiShotPattern;
        float _cooldownTimer;
};

}  // namespace ecs

#endif /* !SHOOTINGSTATSCOMPONENT_HPP_ */
