/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnSystem
*/

#include "SpawnSystem.hpp"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include "../../components/temporary/SpawnIntentComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"

namespace ecs {

SpawnSystem::SpawnSystem() {
}

bool SpawnSystem::isPositionFree(
    Entity newEntity,
    const math::Vector2f& position,
    const std::vector<std::shared_ptr<ColliderComponent>>& newColliders,
    std::shared_ptr<TransformComponent> newTransform,
    std::shared_ptr<Registry> registry
) {
    if (newColliders.empty() || !newTransform) {
        return true;
    }

    View<ColliderComponent, TransformComponent> view(registry);
    for (auto otherEntity : view) {
        if (otherEntity == newEntity) continue;

        auto otherColliders = registry->getComponents<ColliderComponent>(otherEntity);
        auto otherTransform = registry->getComponent<TransformComponent>(otherEntity);

        if (otherColliders.empty() || !otherTransform) continue;

        for (auto newCollider : newColliders) {
            math::FRect newHitbox = newCollider->getHitbox(
                position, newTransform->getScale(), newTransform->getRotation());

            for (auto otherCollider : otherColliders) {
                if (otherCollider->getType() != CollisionType::Solid) {
                    continue;
                }

                math::FRect otherHitbox = otherCollider->getHitbox(
                    otherTransform->getPosition(),
                    otherTransform->getScale(),
                    otherTransform->getRotation()
                );

                if (newHitbox.intersects(otherHitbox)) {
                    return false;
                }
            }
        }
    }
    return true;
}

math::Vector2f SpawnSystem::findNearestFreePosition(
    Entity newEntity,
    const math::Vector2f& originalPosition,
    std::shared_ptr<Registry> registry,
    float stepSize
) {
    auto newColliders = registry->getComponents<ColliderComponent>(newEntity);
    auto newTransform = registry->getComponent<TransformComponent>(newEntity);

    math::Vector2f position = originalPosition;

    if (isPositionFree(newEntity, position, newColliders, newTransform, registry)
        && position.getY() >= 0 &&
        position.getY() <= constants::MAX_HEIGHT) {
        return position;
    }

    float optimizedStep = std::max(stepSize, 30.0f);

    for (int yOffset = static_cast<int>(optimizedStep); yOffset <= 200;
        yOffset += static_cast<int>(optimizedStep)) {
        for (int xOffset = -200; xOffset <= 200; xOffset += static_cast<int>(optimizedStep)) {
            if (xOffset == 0 && yOffset == 0) continue;

            float newY = originalPosition.getY() - static_cast<float>(yOffset);
            if (newY >= 0 && newY <= constants::MAX_HEIGHT) {
                position.setX(originalPosition.getX() + static_cast<float>(xOffset));
                position.setY(newY);

                if (isPositionFree(
                    newEntity, position, newColliders, newTransform, registry)) {
                    return position;
                }
            }

            newY = originalPosition.getY() + static_cast<float>(yOffset);
            if (newY >= 0 && newY <= constants::MAX_HEIGHT) {
                position.setX(originalPosition.getX() + static_cast<float>(xOffset));
                position.setY(newY);

                if (isPositionFree(
                    newEntity, position, newColliders, newTransform, registry)) {
                    return position;
                }
            }
        }
    }

    position = originalPosition;
    position.setY(-constants::MAX_HEIGHT);
    return position;
}

void SpawnSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    auto gameZoneView = registry->view<GameZoneComponent>();

    auto gameZoneEntity = *gameZoneView.begin();
    auto gameZoneTransform = registry->getComponent<TransformComponent>(gameZoneEntity);
    auto gameZonePosition = gameZoneTransform->getPosition();

    auto spawnView = registry->view<SpawnIntentComponent>();

    for (auto entityId : spawnView) {
        auto spawnRequests = registry->getComponents<SpawnIntentComponent>(entityId);

        for (auto spawnRequest : spawnRequests) {
            const float xTrigger = spawnRequest->getGameViewXTrigger();
            if (xTrigger > gameZonePosition.getX())
                continue;

            const std::string prefabName = spawnRequest->getPrefabName();
            const math::Vector2f position = spawnRequest->getPosition();
            const EntityCreationContext context = spawnRequest->getCreationContext();

            const math::Vector2f realPosition(
                gameZonePosition.getX() + position.getX(),
                position.getY()
            );

            auto prefabManager = resourceManager->get<EntityPrefabManager>();
            auto newEntity = prefabManager->createEntityFromPrefab(
                prefabName,
                registry,
                context
            );

            auto transform = registry->getComponent<TransformComponent>(newEntity);
            if (transform) {
                math::Vector2f spawnPosition = realPosition;
                transform->setPosition(spawnPosition);

                auto newColliders = registry->getComponents<ColliderComponent>(newEntity);
                if (!isPositionFree(
                    newEntity, spawnPosition, newColliders, transform, registry)) {
                    spawnPosition =
                        findNearestFreePosition(newEntity, spawnPosition, registry);
                    transform->setPosition(spawnPosition);
                }
            }
            registry->removeOneComponent<SpawnIntentComponent>(entityId);
        }
    }
}

}
