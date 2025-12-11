/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Conversions
*/

#include <memory>
#include <vector>
#include <string>
#include "Server.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/SpeedComponent.hpp"
#include "../common/components/permanent/HealthComponent.hpp"
#include "../common/components/permanent/ColliderComponent.hpp"
#include "../common/components/permanent/ShootingStatsComponent.hpp"
#include "../common/components/permanent/ScoreComponent.hpp"
#include "../common/components/permanent/AIMovementPatternComponent.hpp"
#include "../common/components/permanent/DamageComponent.hpp"
#include "../common/components/permanent/LifetimeComponent.hpp"
#include "../common/components/permanent/VelocityComponent.hpp"
#include "../common/components/tags/AIMoverTag.hpp"
#include "../common/components/tags/AIShooterTag.hpp"
#include "../common/components/tags/ControllableTag.hpp"
#include "../common/components/tags/EnnemyProjectileTag.hpp"
#include "../common/components/tags/GameZoneColliderTag.hpp"
#include "../common/components/tags/MobTag.hpp"
#include "../common/components/tags/ObstacleTag.hpp"
#include "../common/components/tags/PlayerProjectileTag.hpp"
#include "../common/components/tags/ScoreTag.hpp"
#include "../common/components/tags/ShooterTag.hpp"
#include "../common/components/tags/ProjectilePassThroughTag.hpp"
#include "../common/components/permanent/ProjectilePrefabComponent.hpp"
#include "../common/components/permanent/NetworkIdComponent.hpp"

std::vector<uint64_t> rserv::Server::convertTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::PlayerTag>(i)) {
        data.push_back(static_cast<uint64_t>(PLAYER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertTransformComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::TransformComponent>(i)) {
        auto transform = registry->getComponent<ecs::TransformComponent>(i);
        data.push_back(static_cast<uint64_t>(TRANSFORM));
        data.push_back(static_cast<uint64_t>(transform->getPosition().getX()));
        data.push_back(static_cast<uint64_t>(transform->getPosition().getY()));
        data.push_back(static_cast<uint64_t>(transform->getRotation()));
        data.push_back(static_cast<uint64_t>(transform->getScale().getX()));
        data.push_back(static_cast<uint64_t>(transform->getScale().getY()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertSpeedComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::SpeedComponent>(i)) {
        auto speed = registry->getComponent<ecs::SpeedComponent>(i);
        data.push_back(static_cast<uint64_t>(SPEED));
        data.push_back(static_cast<uint64_t>(speed->getSpeed()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertHealthComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::HealthComponent>(i)) {
        auto health = registry->getComponent<ecs::HealthComponent>(i);
        data.push_back(static_cast<uint64_t>(HEALTH));
        data.push_back(static_cast<uint64_t>(health->getHealth()));
        data.push_back(static_cast<uint64_t>(health->getBaseHealth()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertColliderComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ColliderComponent>(i)) {
        auto collider = registry->getComponent<ecs::ColliderComponent>(i);
        data.push_back(static_cast<uint64_t>(COLLIDER));
        data.push_back(static_cast<uint64_t>(collider->getOffset().getX()));
        data.push_back(static_cast<uint64_t>(collider->getOffset().getY()));
        data.push_back(static_cast<uint64_t>(collider->getSize().getX()));
        data.push_back(static_cast<uint64_t>(collider->getSize().getY()));
        data.push_back(static_cast<uint64_t>(collider->getType()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertShootStatComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ShootingStatsComponent>(i)) {
        auto shootStats = registry->getComponent<ecs::ShootingStatsComponent>(i);
        data.push_back(static_cast<uint64_t>(SHOOTING_STATS));
        data.push_back(static_cast<uint64_t>(shootStats->getFireRate()));
        data.push_back(static_cast<uint64_t>(shootStats->getCooldownTimer()));
        data.push_back(static_cast<uint64_t>(shootStats->getMultiShotPattern().shotCount));
        data.push_back(static_cast<uint64_t>(shootStats->getMultiShotPattern().angleSpread));
        data.push_back(static_cast<uint64_t>(
            shootStats->getMultiShotPattern().offsetDistance));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertScoreComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ScoreComponent>(i)) {
        auto scoreComp = registry->getComponent<ecs::ScoreComponent>(i);
        data.push_back(static_cast<uint64_t>(SCORE));
        data.push_back(static_cast<uint64_t>(scoreComp->getScore()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertAIMovementPatternComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::AIMovementPatternComponent>(i)) {
        auto pattern = registry->getComponent<ecs::AIMovementPatternComponent>(i);
        data.push_back(static_cast<uint64_t>(AI_MOVEMENT_PATTERN));
        data.push_back(static_cast<uint64_t>(pattern->getPattern()));
        data.push_back(static_cast<uint64_t>(pattern->getZigzagAmplitude()));
        data.push_back(static_cast<uint64_t>(pattern->getZigzagFrequency()));
        data.push_back(static_cast<uint64_t>(pattern->getDetectionRange()));
        data.push_back(static_cast<uint64_t>(pattern->getVerticalDeadzone()));
        data.push_back(static_cast<uint64_t>(pattern->getTimer()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertDamageComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::DamageComponent>(i)) {
        auto damageComp = registry->getComponent<ecs::DamageComponent>(i);
        data.push_back(static_cast<uint64_t>(DAMAGE));
        data.push_back(static_cast<uint64_t>(damageComp->getDamage()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertLifetimeComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::LifetimeComponent>(i)) {
        auto lifetimeComp = registry->getComponent<ecs::LifetimeComponent>(i);
        data.push_back(static_cast<uint64_t>(LIFETIME));
        data.push_back(static_cast<uint64_t>(lifetimeComp->getLifetime()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertVelocityComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::VelocityComponent>(i)) {
        auto velocity = registry->getComponent<ecs::VelocityComponent>(i);
        data.push_back(static_cast<uint64_t>(VELOCITY));
        data.push_back(static_cast<uint64_t>(velocity->getVelocity().getX()));
        data.push_back(static_cast<uint64_t>(velocity->getVelocity().getY()));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertAIMoverTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::AIMoverTag>(i)) {
        data.push_back(static_cast<uint64_t>(AI_MOVER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertAIShooterTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::AIShooterTag>(i)) {
        data.push_back(static_cast<uint64_t>(AI_SHOOTER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertControllableTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ControllableTag>(i)) {
        data.push_back(static_cast<uint64_t>(CONTROLLABLE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertEnemyProjectileTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::EnnemyProjectileTag>(i)) {
        data.push_back(static_cast<uint64_t>(ENEMY_PROJECTILE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertGameZoneColliderTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::GameZoneColliderTag>(i)) {
        data.push_back(static_cast<uint64_t>(GAME_ZONE_COLLIDER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertMobTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::MobTag>(i)) {
        data.push_back(static_cast<uint64_t>(MOB_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertObstacleTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ObstacleTag>(i)) {
        data.push_back(static_cast<uint64_t>(OBSTACLE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertPlayerProjectileTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::PlayerProjectileTag>(i)) {
        data.push_back(static_cast<uint64_t>(PLAYER_PROJECTILE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertScoreTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ScoreTag>(i)) {
        data.push_back(static_cast<uint64_t>(SCORE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertShooterTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ShooterTag>(i)) {
        data.push_back(static_cast<uint64_t>(SHOOTER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertProjectilePassThroughTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ProjectilePassThroughTag>(i)) {
        data.push_back(static_cast<uint64_t>(PROJECTILE_PASS_THROUGH_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertProjectilePrefabComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::ProjectilePrefabComponent>(i)) {
        data.push_back(static_cast<uint64_t>(PROJECTILE_PREFAB));
        std::string prefabName =
            registry->getComponent<ecs::ProjectilePrefabComponent>(i)->getPrefabName();
        for (char c : prefabName) {
            data.push_back(static_cast<uint64_t>(c));
        }
        data.push_back(static_cast<uint64_t>('\r'));
        data.push_back(static_cast<uint64_t>('\n'));
        data.push_back(static_cast<uint64_t>('\0'));
    }
    return data;
}

std::vector<uint64_t> rserv::Server::convertNetworkIdComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry->hasComponent<ecs::NetworkIdComponent>(i)) {
        auto networkIdComp = registry->getComponent<ecs::NetworkIdComponent>(i);
        data.push_back(static_cast<uint64_t>(NETWORK_ID));
        data.push_back(static_cast<uint64_t>(networkIdComp->getNetworkId()));
    }
    return data;
}
