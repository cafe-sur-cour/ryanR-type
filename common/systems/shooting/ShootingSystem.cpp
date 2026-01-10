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
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/permanent/OwnerComponent.hpp"
#include "../../constants.hpp"
#include "../../../client/components/rendering/RectangleRenderComponent.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"
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
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        auto projectilePrefabComponent =
            registry->getComponent<ProjectilePrefabComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!shootingStats || !projectilePrefabComponent || !transform) {
            continue;
        }
        auto intent = registry->getComponent<ShootIntentComponent>(entityId);
        float baseAngle = intent ? intent->getAngle() : 0.0f;

        if (!shootingStats->canShoot()) {
            continue;
        }

        registry->removeOneComponent<ShootIntentComponent>(entityId);

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

        if (pattern.shotCount == 1) {
            spawnProjectile(registry, resourceManager, prefabName, spawnPos,
                baseAngle, entityId);
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

                spawnProjectile(registry, resourceManager,
                    prefabName, offsetPosition, angle, entityId);
            }
        }

        shootingStats->resetCooldown();
    }

    auto cooldownView = registry->view<ShootingStatsComponent>();
    for (auto entityId : cooldownView) {
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        shootingStats->updateCooldown(deltaTime);
    }
}

void ShootingSystem::spawnProjectile(
    std::shared_ptr<Registry> registry,
    std::shared_ptr<ResourceManager> resourceManager,
    const std::string& prefabName,
    const math::Vector2f &position,
    float angle,
    ecs::Entity shooterEntity
) {
    auto prefabManager = resourceManager->get<EntityPrefabManager>();
    Entity projectileEntity = prefabManager->createEntityFromPrefab(
        prefabName, registry, ecs::EntityCreationContext::forServer()
    );

    registry->addComponent(projectileEntity, std::make_shared<OwnerComponent>(shooterEntity));

    auto transform = registry->getComponent<TransformComponent>(projectileEntity);
    if (transform) {
        transform->setPosition(position);
    }

    float speed = 0.0f;
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
