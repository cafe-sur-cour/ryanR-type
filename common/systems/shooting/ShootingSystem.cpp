/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingSystem
*/

#include <memory>
#include <cmath>
#include "ShootingSystem.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../../client/components/rendering/RectangleRenderComponent.hpp"

namespace ecs {

ShootingSystem::ShootingSystem() {
}

void ShootingSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;

    auto view = registry->view<
        ShootIntentComponent,
        ShootingStatsComponent,
        TransformComponent
    >();

    for (auto entityId : view) {
        auto hasShootIntent = registry->hasComponent<ShootIntentComponent>(entityId);
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!hasShootIntent || !shootingStats || !transform)
            continue;

        shootingStats->updateCooldown(deltaTime);

        if (shootingStats->canShoot()) {
            auto projectilePrefab = shootingStats->getProjectilePrefab();

            shootingStats->resetCooldown();

            auto pattern = shootingStats->getMultiShotPattern();
            float speed = shootingStats->getProjectileSpeed();

            math::Vector2f spawnPos = transform->getPosition();

            float baseAngle = 0.0f;

            if (pattern.shotCount == 1) {
                spawnProjectile(registry, projectilePrefab, spawnPos, baseAngle, speed);
            } else {
                float totalSpread = pattern.angleSpread * (pattern.shotCount - 1);
                float startAngle = baseAngle - (totalSpread / 2.0f);

                for (int i = 0; i < pattern.shotCount; i++) {
                    float angle = startAngle + (i * pattern.angleSpread);

                    math::Vector2f offsetPosition = spawnPos;
                    if (pattern.offsetDistance > 0.0f) {
                        float offsetAngle = angle * M_PI / 180.0f;
                        offsetPosition = math::Vector2f(
                            spawnPos.getX() + pattern.offsetDistance * std::cos(offsetAngle),
                            spawnPos.getY() + pattern.offsetDistance * std::sin(offsetAngle)
                        );
                    }

                    spawnProjectile(registry, projectilePrefab, offsetPosition, angle, speed);
                }
            }

            registry->removeComponent<ShootIntentComponent>(entityId);
        }
    }
}

void ShootingSystem::spawnProjectile(
    std::shared_ptr<Registry> registry,
    std::shared_ptr<IPrefab> projectilePrefab,
    const math::Vector2f &position,
    float angle,
    float speed
) {
    Entity projectileEntity;

    if (!projectilePrefab) {
        projectileEntity = registry->createEntity();
        auto transform = std::make_shared<TransformComponent>();
        registry->addComponent(projectileEntity, transform);
        auto velocity = std::make_shared<VelocityComponent>();
        registry->addComponent(projectileEntity, velocity);
        auto render = std::make_shared<RectangleRenderComponent>(
            gfx::color_t{255, 215, 0},
            10.0f,
            10.0f
        );
        registry->addComponent(projectileEntity, render);
    } else {
        projectileEntity = projectilePrefab->instantiate(registry);
    }

    registry->addComponent(projectileEntity, std::make_shared<ProjectileTag>());

    auto transform = registry->getComponent<TransformComponent>(projectileEntity);
    if (transform) {
        transform->setPosition(position);
    }

    auto velocity = registry->getComponent<VelocityComponent>(projectileEntity);
    if (velocity) {
        math::Vector2f projectileVelocity = calculateProjectileVelocity(angle, speed);
        velocity->setVelocity(projectileVelocity);
    }
}

math::Vector2f ShootingSystem::calculateProjectileVelocity(
    float angle,
    float speed
) {
    float angleRad = angle * M_PI / 180.0f;

    float vx = speed * std::cos(angleRad);
    float vy = speed * std::sin(angleRad);

    return math::Vector2f(vx, vy);
}

}  // namespace ecs
