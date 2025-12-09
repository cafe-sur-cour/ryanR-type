/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathSystem
*/

#include "DeathSystem.hpp"
#include <memory>
#include <exception>
#include <string>
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"
#include "../../../client/components/temporary/SoundIntentComponent.hpp"

namespace ecs {

DeathSystem::DeathSystem() {
}

void DeathSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    auto view = registry->view<DeathIntentComponent>();

    for (auto entityId : view) {
        std::string explosionPrefab = "";
        if (registry->hasComponent<MobTag>(entityId)) {
            explosionPrefab = constants::SMALL_EXPLOSION;
        } else if (registry->hasComponent<PlayerTag>(entityId)) {
            explosionPrefab = constants::BIG_EXPLOSION;
        }
        if (!explosionPrefab.empty()) {
            spawnExplosionAtMobCenter(resourceManager, registry, entityId, explosionPrefab);
        }
        registry->destroyEntity(entityId);
    }
}

math::Vector2f DeathSystem::getFirstHitboxCenter(
    std::shared_ptr<Registry> registry,
    ecs::Entity entity
) {
    auto transform = registry->getComponent<TransformComponent>(entity);
    auto collider = registry->getComponent<ColliderComponent>(entity);
    if (!transform || !collider) {
        return math::Vector2f(0.0f, 0.0f);
    }
    math::Vector2f position = transform->getPosition();
    math::Vector2f offset = collider->getOffset();
    math::Vector2f size = collider->getSize();
    math::Vector2f scale = transform->getScale();

    return position + offset + math::Vector2f(
        size.getX() * scale.getX() / 2.0f,
        size.getY() * scale.getY() / 2.0f
    );
}

void DeathSystem::spawnExplosionAtMobCenter(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    ecs::Entity mobEntity,
    const std::string& prefabName
) {
    auto prefabManager = resourceManager->get<EntityPrefabManager>();
    if (!prefabManager) return;

    try {
        ecs::Entity explosionEntity =
            prefabManager->createEntityFromPrefab(prefabName, registry);

        auto explosionTransform =
            registry->getComponent<TransformComponent>(explosionEntity);
        auto explosionCollider =
            registry->getComponent<ColliderComponent>(explosionEntity);
        if (explosionTransform && explosionCollider) {
            math::Vector2f mobCenter = getFirstHitboxCenter(registry, mobEntity);
            math::Vector2f explosionOffset = explosionCollider->getOffset();
            math::Vector2f explosionSize = explosionCollider->getSize();
            math::Vector2f explosionScale = explosionTransform->getScale();
            math::Vector2f explosionCenterOffset = explosionOffset + math::Vector2f(
                explosionSize.getX() * explosionScale.getX() / 2.0f,
                explosionSize.getY() * explosionScale.getY() / 2.0f
            );
            math::Vector2f explosionPosition = mobCenter - explosionCenterOffset;
            explosionTransform->setPosition(explosionPosition);
        }
    } catch (const std::exception& e) {
        (void) e;
    }
}

}  // namespace ecs
