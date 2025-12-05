/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingSystem
*/

#ifndef SHOOTINGSYSTEM_HPP_
#define SHOOTINGSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include "../../components/base/IComponent.hpp"
#include "../../components/temporary/ShootIntentComponent.hpp"
#include "../../components/permanent/ShootingStatsComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../Prefab/IPrefab.hpp"
#include <cmath>
#include <string>

namespace ecs {

class ShootingSystem : public ASystem {
    public:
        ShootingSystem();
        ~ShootingSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void spawnProjectile(
            std::shared_ptr<Registry> registry,
            std::shared_ptr<IPrefab> prefab,
            const math::Vector2f &position,
            float angle
        );

        math::Vector2f calculateProjectileVelocity(
            float angle,
            float speed
        );
};

}  // namespace ecs

#endif /* !SHOOTINGSYSTEM_HPP_ */
