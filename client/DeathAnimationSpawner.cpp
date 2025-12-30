/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathAnimationSpawner
*/

#include "DeathAnimationSpawner.hpp"
#include <memory>
#include <exception>
#include <string>
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/ColliderComponent.hpp"
#include "../common/components/tags/MobTag.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/components/tags/ClientEffectTag.hpp"
#include "../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../common/ECS/entity/EntityCreationContext.hpp"
#include "../common/constants.hpp"
#include "./components/rendering/PrefabAfterDeath.hpp"

void DeathAnimationSpawner::spawnDeathAnimation(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity entity
) {
    std::string prefabAfterDeathName = "";
    if (registry->hasComponent<ecs::PrefabAfterDeath>(entity)) {
        auto prefabComp = registry->getComponent<ecs::PrefabAfterDeath>(entity);
        if (prefabComp) {
            prefabAfterDeathName = prefabComp->getPrefabName();
        }
    }

    if (prefabAfterDeathName.empty())
        return;

    auto prefabManager = resourceManager->get<EntityPrefabManager>();
    if (!prefabManager)
        return;

    try {
        ecs::Entity animationEntity =
            prefabManager->createEntityFromPrefab(prefabAfterDeathName, registry,
                ecs::EntityCreationContext::forLocalClient());

        auto animationTransform =
            registry->getComponent<ecs::TransformComponent>(animationEntity);
        auto animationCollider =
            registry->getComponent<ecs::ColliderComponent>(animationEntity);
        if (animationTransform && animationCollider) {
            math::Vector2f entityCenter = getFirstHitboxCenter(registry, entity);
            math::Vector2f animationOffset = animationCollider->getOffset();
            math::Vector2f animationSize = animationCollider->getSize();
            math::Vector2f animationScale = animationTransform->getScale();
            math::Vector2f animationCenterOffset = animationOffset + math::Vector2f(
                animationSize.getX() * animationScale.getX() / 2.0f,
                animationSize.getY() * animationScale.getY() / 2.0f
            );
            math::Vector2f animationPosition = entityCenter - animationCenterOffset;
            animationTransform->setPosition(animationPosition);
        }
    } catch (const std::exception& e) {
        (void) e;
    }
}

math::Vector2f DeathAnimationSpawner::getFirstHitboxCenter(
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity entity
) {
    auto transform = registry->getComponent<ecs::TransformComponent>(entity);
    auto collider = registry->getComponent<ecs::ColliderComponent>(entity);
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
