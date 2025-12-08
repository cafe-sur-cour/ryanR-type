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
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"

namespace ecs {

SpawnSystem::SpawnSystem() {
}

void SpawnSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    auto view = registry->view<SpawnIntentComponent>();

    for (auto entityId : view) {
        auto spawnRequest = registry->getComponent<SpawnIntentComponent>(entityId);

        const std::string prefabName = spawnRequest->getPrefabName();
        const math::Vector2f position = spawnRequest->getPosition();

        auto prefabManager = resourceManager->get<EntityPrefabManager>();
        auto newEntity = prefabManager->createEntityFromPrefab(prefabName, registry);

        auto transform = registry->getComponent<TransformComponent>(newEntity);
        transform->setPosition(position);

        registry->removeComponent<SpawnIntentComponent>(entityId);
    }
}

}
