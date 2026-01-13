
/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneStopSystem
*/

#include "GameZoneStopSystem.hpp"
#include <memory>
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/tags/GameZoneStopTag.hpp"
#include "../../types/FRect.hpp"

namespace ecs {

GameZoneStopSystem::GameZoneStopSystem() {
}

void GameZoneStopSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

    auto playerView = registry->view<PlayerTag, TransformComponent, ColliderComponent>();
    std::shared_ptr<TransformComponent> playerTransform = nullptr;
    std::shared_ptr<ColliderComponent> playerCollider = nullptr;

    for (auto entity : playerView) {
        playerTransform = registry->getComponent<TransformComponent>(entity);
        playerCollider = registry->getComponent<ColliderComponent>(entity);
        break;
    }

    if (!playerTransform || !playerCollider) {
        return;
    }

    auto playerHitbox = playerCollider->getScaledHitbox(
        playerTransform->getPosition(),
        playerTransform->getScale()
    );

    auto stopZoneView = registry->view<GameZoneStopTag, TransformComponent,
        ColliderComponent>();
    for (auto stopZoneEntity : stopZoneView) {
        auto stopZoneTransform = registry->getComponent<TransformComponent>(stopZoneEntity);
        auto stopZoneCollider = registry->getComponent<ColliderComponent>(stopZoneEntity);

        if (!stopZoneTransform || !stopZoneCollider) {
            continue;
        }

        auto stopZoneHitbox = stopZoneCollider->getScaledHitbox(
            stopZoneTransform->getPosition(),
            stopZoneTransform->getScale()
        );
        if (playerHitbox.intersects(stopZoneHitbox)) {
            auto gameZoneView = registry->view<GameZoneComponent, VelocityComponent>();
            for (auto gameZoneEntity : gameZoneView) {
                auto velocityComp = registry->getComponent<VelocityComponent>(gameZoneEntity);
                if (velocityComp) {
                    velocityComp->setVelocity(math::Vector2f(0.0f, 0.0f));
                }
            }
            registry->destroyEntity(stopZoneEntity);
            break;
        }
    }
}

}
