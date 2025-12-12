/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnSystem
*/

#include "SpawnSystem.hpp"
#include <memory>
#include <string>
#include "../../components/temporary/SpawnIntentComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include <iostream>
namespace ecs {

SpawnSystem::SpawnSystem() {
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
                transform->setPosition(realPosition);
            }
            registry->removeOneComponent<SpawnIntentComponent>(entityId);
        }
    }
}

}
