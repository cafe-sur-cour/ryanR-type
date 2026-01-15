/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingApiFunctions
*/

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include <sol/sol.hpp>
#include "ScriptingSystem.hpp"
#include "../../components/permanent/EntityPartsComponent.hpp"
#include "../../components/permanent/CompositeEntityComponent.hpp"
#include "../../components/temporary/SpawnIntentComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../ECS/view/View.hpp"
#include "../../components/temporary/MovementIntentComponent.hpp"
#include "../../components/temporary/ShootIntentComponent.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/ProjectilePrefabComponent.hpp"
#include "../../components/permanent/AnimationStateComponent.hpp"
#include "../../components/tags/ForceTag.hpp"
#include "../../components/permanent/ShootingStatsComponent.hpp"
#include "../../components/permanent/HealthComponent.hpp"
#include "../../components/permanent/ChargedShotComponent.hpp"
#include "../../components/permanent/InvulnerableComponent.hpp"
#include "../../components/tags/MobTag.hpp"

namespace ecs {

void ScriptingSystem::bindAPI() {
    lua.set_function(constants::PRINT_FUNCTION, [](const std::string& msg) {
        std::cout << "[Lua] " << msg << std::endl;
    });

    lua.set_function(constants::CREATE_MOVE_INTENT_FUNCTION,
        [this](Entity e, float x, float y) {
        auto intent = std::make_shared<ecs::MovementIntentComponent>
            (math::Vector2f(x, y), true);
        registry->addComponent<ecs::MovementIntentComponent>(e, intent);
    });

    lua.set_function(constants::GET_ENTITY_POSITION_FUNCTION,
            [this](Entity e) -> std::tuple<float, float> {
        if (registry->hasComponent<TransformComponent>(e)) {
            auto transform = registry->getComponent<TransformComponent>(e);
            auto pos = transform->getPosition();
            return {pos.getX(), pos.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::SET_ENTITY_ROTATION_FUNCTION,
        [this](size_t entityId, float rotation) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<TransformComponent>(e)) {
            auto transform = registry->getComponent<TransformComponent>(e);
            transform->setRotation(rotation);
        }
    });

    lua.set_function(constants::GET_ENTITY_VELOCITY_FUNCTION,
        [this](size_t entityId) -> std::tuple<float, float> {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<VelocityComponent>(e)) {
            auto velocity = registry->getComponent<VelocityComponent>(e);
            auto vel = velocity->getVelocity();
            return {vel.getX(), vel.getY()};
        }
        return {0.0f, 0.0f};
    });


    lua.set_function(constants::GET_NEAREST_PLAYER_POSITION_FUNCTION,
            [this](size_t e) -> std::tuple<float, float> {
        Entity entity = static_cast<Entity>(e);
        if (!registry->hasComponent<TransformComponent>(entity)) {
            return {0.0f, 0.0f};
        }
        auto myTransform = registry->getComponent<TransformComponent>(entity);
        auto myPos = myTransform->getPosition();
        auto view = registry->view<PlayerTag, TransformComponent>();
        float minDist = std::numeric_limits<float>::max();
        math::Vector2f closestPos(0.0f, 0.0f);
        bool found = false;
        for (auto entityId : view) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            auto pos = transform->getPosition();
            float dx = pos.getX() - myPos.getX();
            float dy = pos.getY() - myPos.getY();
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < minDist) {
                minDist = dist;
                closestPos = pos;
                found = true;
            }
        }
        if (!found)
            return {0.0f, 0.0f};
        return {closestPos.getX(), closestPos.getY()};
    });

    lua.set_function(constants::GET_ENTITY_SPEED_FUNCTION, [this](Entity e) -> float {
        if (registry->hasComponent<SpeedComponent>(e)) {
            auto speedComp = registry->getComponent<SpeedComponent>(e);
            return speedComp->getSpeed();
        }
        return 0.0f;
    });

    lua.set_function(constants::SET_ENTITY_SPEED_FUNCTION,
        [this](Entity e, float speed) {
        if (registry->hasComponent<SpeedComponent>(e)) {
            auto speedComp = registry->getComponent<SpeedComponent>(e);
            speedComp->setSpeed(speed);
        }
    });

    lua.set_function(constants::GET_MAX_CHARGE_FUNCTION,
        [this](Entity e) -> float {
        if (registry->hasComponent<ChargedShotComponent>(e)) {
            auto chargedShotComp = registry->getComponent<ChargedShotComponent>(e);
            return chargedShotComp->getMaxCharge();
        }
        return 0.0f;
    });

    lua.set_function(constants::SET_MAX_CHARGE_FUNCTION,
        [this](Entity e, float maxCharge) {
        if (registry->hasComponent<ChargedShotComponent>(e)) {
            auto chargedShotComp = registry->getComponent<ChargedShotComponent>(e);
            chargedShotComp->setMaxCharge(maxCharge);
        }
    });

    lua.set_function(constants::CREATE_SHOOT_INTENT_FUNCTION,
        [this](Entity e, float angle) -> bool {
        if (registry->hasComponent<ShootIntentComponent>(e))
            return false;
        auto intent = std::make_shared<ecs::ShootIntentComponent>(angle);
        registry->addComponent<ecs::ShootIntentComponent>(e, intent);
        return true;
    });

    lua.set_function(constants::SET_ANIMATION_STATE_FUNCTION,
        [this](Entity e, const std::string& newState) {
        if (registry->hasComponent<AnimationStateComponent>(e)) {
            auto animStateComp = registry->getComponent<AnimationStateComponent>(e);
            animStateComp->setCurrentState(newState);
            return;
        } else {
            auto animStateComp = std::make_shared<AnimationStateComponent>(newState);
            registry->addComponent<AnimationStateComponent>(e, animStateComp);
        }
    });

    lua.set_function(constants::GET_ENTITY_ID_FUNCTION, [](Entity e) -> size_t {
        return static_cast<size_t>(e);
    });

    lua.set_function(constants::ADD_PART_ID_FUNCTION,
        [this](size_t entityId, size_t partId) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<EntityPartsComponent>(e)) {
            auto comp = registry->getComponent<EntityPartsComponent>(e);
            comp->partIds.push_back(partId);
        }
    });

    lua.set_function(constants::SET_PARENT_ID_FUNCTION,
        [this](size_t entityId, size_t parentId) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<CompositeEntityComponent>(e)) {
            auto comp = registry->getComponent<CompositeEntityComponent>(e);
            comp->setParentId(parentId);
        } else {
            auto comp = std::make_shared<CompositeEntityComponent>(parentId);
            registry->addComponent<CompositeEntityComponent>(e, comp);
        }
    });

    lua.set_function(constants::SPAWN_ENTITY_FUNCTION,
        [this](const std::string& prefabName, float x, float y) -> size_t {
        if (!resourceManager || !registry)
            return 0;
        auto prefabManager = resourceManager->get<EntityPrefabManager>();
        if (!prefabManager)
            return 0;
        Entity newEntity = prefabManager->createEntityFromPrefab(
            prefabName,
            registry,
            ecs::EntityCreationContext::forServer()
        );
        if (registry->hasComponent<TransformComponent>(newEntity)) {
            auto transform = registry->getComponent<TransformComponent>(newEntity);
            transform->setPosition(math::Vector2f(x, y));
        }
        return static_cast<size_t>(newEntity);
    });

    lua.set_function(constants::GET_ENTITY_PARTS_FUNCTION,
        [this](size_t entityId) -> std::vector<size_t> {
        Entity e = static_cast<Entity>(entityId);
        std::vector<size_t> partIds;
        if (registry->hasComponent<EntityPartsComponent>(e)) {
            auto comp = registry->getComponent<EntityPartsComponent>(e);
            partIds = comp->partIds;
        }
        return partIds;
    });
    lua.set_function(constants::CREATE_DEATH_INTENT_FUNCTION,
        [this](size_t entityId, size_t sourceId) {
        Entity e = static_cast<Entity>(entityId);
        Entity source = static_cast<Entity>(sourceId);
        auto deathIntent = std::make_shared<DeathIntentComponent>(source);
        registry->addComponent<DeathIntentComponent>(e, deathIntent);
    });

    lua.set_function(constants::IS_ENTITY_ALIVE_FUNCTION, [this](size_t entityId) -> bool {
        Entity e = static_cast<Entity>(entityId);
        return registry->hasComponent<TransformComponent>(e);
    });

    lua.set_function(constants::GET_PARENT_ID_FUNCTION, [this](size_t entityId) -> size_t {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<CompositeEntityComponent>(e)) {
            auto comp = registry->getComponent<CompositeEntityComponent>(e);
            return comp->getParentId();
        }
        return 0;
    });

    lua.set_function(constants::REMOVE_PART_ID_FUNCTION,
        [this](size_t entityId, size_t partId) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<EntityPartsComponent>(e)) {
            auto comp = registry->getComponent<EntityPartsComponent>(e);
            auto it = std::find(comp->partIds.begin(), comp->partIds.end(), partId);
            if (it != comp->partIds.end()) {
                comp->partIds.erase(it);
            }
        }
    });

    lua.set_function(constants::IS_ENTITY_PLAYER_FUNCTION,
        [this](size_t entityId) {
        Entity e = static_cast<Entity>(entityId);
        return registry->hasComponent<PlayerTag>(e);
    });

    lua.set_function(constants::GET_ENTITY_SIZE_FUNCTION,
        [this](size_t entityId) -> std::tuple<float, float> {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<ColliderComponent>(e) &&
            registry->hasComponent<TransformComponent>(e)) {
            auto hitbox = registry->getComponent<ColliderComponent>(e);
            auto transform = registry->getComponent<TransformComponent>(e);
            auto size = hitbox->getSize();
            auto scale = transform->getScale();
            return {size.getX() * scale.getX(), size.getY() * scale.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::SET_POSITION_FUNCTION,
        [this](size_t entityId, float x, float y) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<TransformComponent>(e)) {
            auto transform = registry->getComponent<TransformComponent>(e);
            transform->setPosition(math::Vector2f(x, y));
        }
    });

    lua.set_function(constants::SET_PROJECTILE_PREFAB_FUNCTION,
        [this](size_t entityId, const std::string& prefabName) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<ProjectilePrefabComponent>(e)) {
            auto shootComp = registry->getComponent<ProjectilePrefabComponent>(e);
            shootComp->setPrefabName(prefabName);
        }
    });

    lua.set_function(constants::COUNT_FORCES_BY_TYPE_FUNCTION,
        [this](size_t playerEntityId, const std::string& forceType) -> int {
        Entity playerEntity = static_cast<Entity>(playerEntityId);
        int count = 0;
        if (!registry->hasComponent<EntityPartsComponent>(playerEntity))
            return 0;
        auto partsComp = registry->getComponent<EntityPartsComponent>(playerEntity);
        for (size_t partId : partsComp->partIds) {
            Entity part = static_cast<Entity>(partId);
            if (registry->hasComponent<ForceTag>(part)) {
                auto forceTag = registry->getComponent<ForceTag>(part);
                if (forceTag->getForceType() == forceType) {
                    count++;
                }
            }
        }
        return count;
    });

    lua.set_function(constants::GET_FORCE_POSITION_BY_TYPE_FUNCTION,
        [this](size_t entityId, size_t playerEntityId, const std::string& forceType) -> int {
        Entity entity = static_cast<Entity>(entityId);
        Entity playerEntity = static_cast<Entity>(playerEntityId);
        int position = 0;

        if (!registry->hasComponent<EntityPartsComponent>(playerEntity))
            return 0;
        auto partsComp = registry->getComponent<EntityPartsComponent>(playerEntity);
        for (size_t partId : partsComp->partIds) {
            Entity part = static_cast<Entity>(partId);
            if (registry->hasComponent<ForceTag>(part)) {
                auto forceTag = registry->getComponent<ForceTag>(part);
                if (forceTag->getForceType() == forceType) {
                    position++;
                    if (part == entity) {
                        return position;
                    }
                }
            }
        }
        return 0;
    });

    lua.set_function(constants::ADD_FORCE_LEVEL_FUNCTION,
        [this](size_t entityId) {
        if (registry->hasComponent<ecs::EntityPartsComponent>(entityId)) {
            std::vector<size_t> partsComp =
                registry->getComponent<ecs::EntityPartsComponent>(entityId)->partIds;

            for (auto partId : partsComp) {
                Entity part = static_cast<Entity>(partId);
                if (registry->hasComponent<ForceTag>(part) &&
                    registry->getComponent<ecs::ForceTag>(part)->getForceType() ==
                        constants::FORCE_TYPE
                    && registry->hasComponent<ecs::ScriptingComponent>(part)) {
                    auto scriptComp = registry->getComponent<ecs::ScriptingComponent>(part);
                    auto forceTag = registry->getComponent<ForceTag>(part);
                    if (scriptComp->hasFunction(constants::ADD_FORCE_LEVEL_FUNCTION)) {
                        sol::function addLevelFunc = scriptComp->
                            getFunction(constants::ADD_FORCE_LEVEL_FUNCTION);
                        addLevelFunc(part);
                    }
                }
            }
        }
    });

    lua.set_function(constants::SET_GAME_ZONE_VELOCITY_FUNCTION,
        [this](float x, float y) {
        auto gameZoneView = registry->view<GameZoneComponent, VelocityComponent>();
        for (auto gameZoneEntity : gameZoneView) {
            auto velocityComp = registry->getComponent<VelocityComponent>(gameZoneEntity);
            if (velocityComp) {
                velocityComp->setVelocity(math::Vector2f(x , y));
            }
        }
    });

    lua.set_function(constants::GET_GAME_ZONE_POSITION_FUNCTION,
        [this]() -> std::tuple<float, float> {
        auto gameZoneView = registry->view<GameZoneComponent>();
        for (auto gameZoneEntity : gameZoneView) {
            if (registry->hasComponent<TransformComponent>(gameZoneEntity)) {
                auto transform = registry->getComponent<TransformComponent>(gameZoneEntity);
                auto pos = transform->getPosition();
                return {pos.getX(), pos.getY()};
            }
            auto gameZoneComp = registry->getComponent<GameZoneComponent>(gameZoneEntity);
            auto zone = gameZoneComp->getZone();
            return {zone.getLeft(), zone.getTop()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::GET_GAME_ZONE_SIZE_FUNCTION,
        [this]() -> std::tuple<float, float> {
        auto gameZoneView = registry->view<GameZoneComponent>();
        for (auto gameZoneEntity : gameZoneView) {
            auto gameZoneComp = registry->getComponent<GameZoneComponent>(gameZoneEntity);
            auto zone = gameZoneComp->getZone();
            return {zone.getWidth(), zone.getHeight()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::GET_GAME_ZONE_VELOCITY_FUNCTION,
        [this]() -> std::tuple<float, float> {
        auto gameZoneView = registry->view<GameZoneComponent, VelocityComponent>();
        for (auto gameZoneEntity : gameZoneView) {
            auto velocityComp = registry->getComponent<VelocityComponent>(gameZoneEntity);
            auto vel = velocityComp->getVelocity();
            return {vel.getX(), vel.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::REVERSE_SHOOT_ORIENTATION_FUNCTION,
        [this](size_t entityId) {
        if (!registry->hasComponent<ShootingStatsComponent>(entityId))
            return;
        auto shootingStatsComp = registry->getComponent<ShootingStatsComponent>(entityId);
        MultiShotPattern pattern = shootingStatsComp->getMultiShotPattern();
        pattern.angleOffset += 180.0f;
        shootingStatsComp->setMultiShotPattern(pattern);
    });

    lua.set_function(constants::SET_INVULNERABLE_FUNCTION,
        [this](size_t entityId, bool isInvulnerable) {
        Entity e = static_cast<Entity>(entityId);
        if (registry->hasComponent<ecs::InvulnerableComponent>(e)) {
            auto invulnerableComp = registry->getComponent<ecs::InvulnerableComponent>(e);
            invulnerableComp->setActive(isInvulnerable);
        }
    });

    lua.set_function(constants::GET_NEAREST_ENEMY_POSITION_FUNCTION,
            [this](size_t e) -> std::tuple<float, float> {
        Entity entity = static_cast<Entity>(e);
        if (!registry->hasComponent<TransformComponent>(entity))
            return {0.0f, 0.0f};
        auto myTransform = registry->getComponent<TransformComponent>(entity);
        auto myPos = myTransform->getPosition();
        auto view = registry->view<TransformComponent, MobTag>();
        float minDist = std::numeric_limits<float>::max();
        math::Vector2f closestPos(0.0f, 0.0f);
        bool found = false;
        for (auto entityId : view) {
            if (entityId == entity)
                continue;
            auto transform = registry->getComponent<TransformComponent>(entityId);
            auto pos = transform->getPosition();
            float dx = pos.getX() - myPos.getX();
            float dy = pos.getY() - myPos.getY();
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist < minDist) {
                minDist = dist;
                closestPos = pos;
                found = true;
            }
        }
        if (!found)
            return {0.0f, 0.0f};
        return {closestPos.getX(), closestPos.getY()};
    });
}

}  // namespace ecs
