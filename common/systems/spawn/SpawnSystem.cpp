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
    std::shared_ptr<Registry> registry
) {
    auto newColliders = registry->getComponents<ColliderComponent>(newEntity);
    auto newTransform = registry->getComponent<TransformComponent>(newEntity);
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
                math::FRect otherHitbox = otherCollider->getHitbox(
                    otherTransform->getPosition(),
                    otherTransform->getScale(),
                    otherTransform->getRotation()
                );

                if (newHitbox.intersects(otherHitbox)) {
                    if (otherCollider->getType() == CollisionType::Solid) {
                        const auto& tagRegistry = TagRegistry::getInstance();
                        std::vector<std::string> newTags =
                            tagRegistry.getTags(registry, newEntity);
                        std::vector<std::string> otherTags =
                            tagRegistry.getTags(registry, otherEntity);

                        if (CollisionRules::getInstance().canCollide(
                            CollisionType::Solid,
                            newTags,
                            otherTags
                        )) {
                            return false;
                        }
                    }
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
    math::Vector2f position = originalPosition;
    float centerY = constants::MAX_HEIGHT / 2.0f;

    if (isPositionFree(newEntity, position, registry) && position.getY() >= 0 &&
        position.getY() <= constants::MAX_HEIGHT) {
        return position;
    }

    for (int yOffset = 0; yOffset <= 100; yOffset += static_cast<int>(stepSize)) {
        for (int xOffset = -100; xOffset <= 100; xOffset += static_cast<int>(stepSize)) {
            if (xOffset == 0 && yOffset == 0) continue;

            float newY = originalPosition.getY() + yOffset;
            if (newY > centerY) newY = centerY - (newY - centerY);

            if (newY < 0 || newY > constants::MAX_HEIGHT) continue;

            position.setX(originalPosition.getX() + xOffset);
            position.setY(newY);

            if (isPositionFree(newEntity, position, registry)) {
                return position;
            }

            newY = originalPosition.getY() - yOffset;
            if (newY > centerY) newY = centerY - (newY - centerY);

            if (newY >= 0 && newY <= constants::MAX_HEIGHT) {
                position.setY(newY);
                if (isPositionFree(newEntity, position, registry)) {
                    return position;
                }
            }
        }
    }

    position = originalPosition;
    position.setY(std::max(0.0f, std::min(constants::MAX_HEIGHT, position.getY())));
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

                if (!isPositionFree(newEntity, spawnPosition, registry)) {
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
