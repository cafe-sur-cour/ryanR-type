/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingSystem
*/

#define _USE_MATH_DEFINES
#include "ShootingSystem.hpp"
#include <memory>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ShootingStatsComponent.hpp"
#include "../../components/permanent/ProjectilePrefabComponent.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/permanent/InteractionConfigComponent.hpp"
#include "../../constants.hpp"
#include "../../../client/components/rendering/RectangleRenderComponent.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
namespace ecs {

ShootingSystem::ShootingSystem() {
}

void ShootingSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    auto view = registry->view<
        ShootIntentComponent,
        ShootingStatsComponent,
        ProjectilePrefabComponent,
        TransformComponent
    >();

    for (auto entityId : view) {
        auto hasShootIntent = registry->hasComponent<ShootIntentComponent>(entityId);
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        auto projectilePrefabComponent = registry->getComponent<ProjectilePrefabComponent>(
            entityId
        );
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!hasShootIntent || !shootingStats || !projectilePrefabComponent || !transform)
            continue;

        registry->removeComponent<ShootIntentComponent>(entityId);

        if (!shootingStats->canShoot())
            continue;

        auto prefabManager = resourceManager->get<EntityPrefabManager>();
        std::string prefabName = projectilePrefabComponent->getPrefabName();
        auto prefab = prefabManager->getPrefab(prefabName);

        auto pattern = shootingStats->getMultiShotPattern();

        math::Vector2f spawnPos = transform->getPosition();

        auto collider = registry->getComponent<ColliderComponent>(entityId);
        if (collider) {
            math::Vector2f colliderCenter =
                collider->getOffset() + (collider->getSize() * 0.5f);
            spawnPos = spawnPos + colliderCenter;
        }

        float baseAngle = 0.0f;

        if (pattern.shotCount == 1) {
            spawnProjectile(registry, prefab, spawnPos, baseAngle, 0.0f);
        } else {
            float totalSpread = pattern.angleSpread * static_cast<float>(
                pattern.shotCount - 1
            );
            float startAngle = baseAngle - (totalSpread / 2.0f);

            for (int i = 0; i < pattern.shotCount; i++) {
                float angle = startAngle + (static_cast<float>(i) * pattern.angleSpread);

                math::Vector2f offsetPosition = spawnPos;
                if (pattern.offsetDistance > 0.0f) {
                    float offsetAngle = angle * static_cast<float>(M_PI) / 180.0f;
                    offsetPosition = math::Vector2f(
                        spawnPos.getX() + pattern.offsetDistance * std::cos(offsetAngle),
                        spawnPos.getY() + pattern.offsetDistance * std::sin(offsetAngle)
                    );
                }

                spawnProjectile(registry, prefab, offsetPosition, angle, 0.0f);
            }
        }

        shootingStats->resetCooldown();
    }

    auto cooldownView = registry->view<ShootingStatsComponent>();
    for (auto entityId : cooldownView) {
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        if (shootingStats) {
            shootingStats->updateCooldown(deltaTime);
        }
    }
}

void ShootingSystem::spawnProjectile(
    std::shared_ptr<Registry> registry,
    std::shared_ptr<IPrefab> prefab,
    const math::Vector2f &position,
    float angle,
    float defaultSpeed
) {
    Entity projectileEntity;

    if (prefab) {
        projectileEntity = prefab->instantiate(registry);
    } else {
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
    }

    registry->addComponent(projectileEntity, std::make_shared<ProjectileTag>());

    InteractionMapping deathMapping;
    deathMapping.targetTags = {"MobTag"};
    deathMapping.actionToOther = constants::DEATHINTENT_ACTION;
    deathMapping.actionToSelf = "";

    std::vector<InteractionMapping> mappings = {deathMapping};
    registry->addComponent(projectileEntity,
        std::make_shared<InteractionConfigComponent>(mappings));

    auto transform = registry->getComponent<TransformComponent>(projectileEntity);
    if (transform) {
        transform->setPosition(position);
    }

    float speed = defaultSpeed;
    auto projectileSpeedComp = registry->getComponent<SpeedComponent>(projectileEntity);
    if (projectileSpeedComp) {
        speed = projectileSpeedComp->getSpeed();
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
    float angleRad = angle * static_cast<float>(M_PI) / 180.0f;

    float vx = speed * std::cos(angleRad);
    float vy = speed * std::sin(angleRad);

    return math::Vector2f(vx, vy);
}

}  // namespace ecs
