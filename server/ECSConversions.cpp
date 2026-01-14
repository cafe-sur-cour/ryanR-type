/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Conversions
*/

#include <memory>
#include <vector>
#include <string>
#include "Lobby.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/SpeedComponent.hpp"
#include "../common/components/permanent/HealthComponent.hpp"
#include "../common/components/permanent/ColliderComponent.hpp"
#include "../common/components/permanent/ShootingStatsComponent.hpp"
#include "../common/components/permanent/ScoreComponent.hpp"
#include "../common/components/permanent/DamageComponent.hpp"
#include "../common/components/permanent/LifetimeComponent.hpp"
#include "../common/components/permanent/VelocityComponent.hpp"
#include "../common/components/tags/ControllableTag.hpp"
#include "../common/components/tags/EnnemyProjectileTag.hpp"
#include "../common/components/tags/GameZoneColliderTag.hpp"
#include "../common/components/tags/MobTag.hpp"
#include "../common/components/tags/ObstacleTag.hpp"
#include "../common/components/tags/PlayerProjectileTag.hpp"
#include "../common/components/tags/ShooterTag.hpp"
#include "../common/components/tags/ProjectilePassThroughTag.hpp"
#include "../common/components/permanent/ProjectilePrefabComponent.hpp"
#include "../common/components/permanent/GameZoneComponent.hpp"
#include "../common/components/permanent/ChargedShotComponent.hpp"
#include "../common/components/permanent/AnimationStateComponent.hpp"
#include "../common/FloatQuantization.hpp"

std::vector<uint64_t> rserv::Lobby::convertTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::PlayerTag>(i)) {
        data.push_back(static_cast<uint64_t>(PLAYER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertTransformComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::TransformComponent>(i)) {
        auto transform = registry->getComponent<ecs::TransformComponent>(i);
        if (transform) {
            data.push_back(static_cast<uint64_t>(TRANSFORM));
            data.push_back(quantization::packPosition(transform->getPosition().getX()));
            data.push_back(quantization::packPosition(transform->getPosition().getY()));
            data.push_back(quantization::packRotation(transform->getRotation()));
            data.push_back(quantization::packScale(transform->getScale().getX()));
            data.push_back(quantization::packScale(transform->getScale().getY()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertSpeedComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::SpeedComponent>(i)) {
        auto speed = registry->getComponent<ecs::SpeedComponent>(i);
        if (speed) {
            data.push_back(static_cast<uint64_t>(SPEED_COMP));
            data.push_back(quantization::packSpeed(speed->getSpeed()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertHealthComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::HealthComponent>(i)) {
        auto health = registry->getComponent<ecs::HealthComponent>(i);
        if (health) {
            data.push_back(static_cast<uint64_t>(HEALTH));
            data.push_back(quantization::packHealth(health->getHealth()));
            data.push_back(quantization::packHealth(health->getBaseHealth()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertColliderComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ColliderComponent>(i)) {
        auto collider = registry->getComponent<ecs::ColliderComponent>(i);
        if (collider) {
            data.push_back(static_cast<uint64_t>(COLLIDER));
            data.push_back(quantization::packPosition(collider->getOffset().getX()));
            data.push_back(quantization::packPosition(collider->getOffset().getY()));
            data.push_back(quantization::packSize(collider->getSize().getX()));
            data.push_back(quantization::packSize(collider->getSize().getY()));
            data.push_back(static_cast<uint64_t>(collider->getType()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertShootStatComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ShootingStatsComponent>(i)) {
        auto shootStats = registry->getComponent<ecs::ShootingStatsComponent>(i);
        if (shootStats) {
            data.push_back(static_cast<uint64_t>(SHOOTING_STATS));
            data.push_back(quantization::packTime(shootStats->getFireRate()));
            data.push_back(quantization::packTime(shootStats->getCooldownTimer()));
            data.push_back(static_cast<uint64_t>(
                shootStats->getMultiShotPattern().shotCount));
            data.push_back(quantization::packAngle(
                shootStats->getMultiShotPattern().angleSpread));
            data.push_back(quantization::packSize(
                shootStats->getMultiShotPattern().offsetDistance));
            data.push_back(quantization::packAngle(
                shootStats->getMultiShotPattern().angleOffset));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertScoreComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ScoreComponent>(i)) {
        auto scoreComp = registry->getComponent<ecs::ScoreComponent>(i);
        if (scoreComp) {
            data.push_back(static_cast<uint64_t>(SCORE));
            data.push_back(static_cast<uint64_t>(scoreComp->getScore()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertDamageComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::DamageComponent>(i)) {
        auto damageComp = registry->getComponent<ecs::DamageComponent>(i);
        if (damageComp) {
            data.push_back(static_cast<uint64_t>(DAMAGE));
            data.push_back(quantization::packDamage(damageComp->getDamage()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertLifetimeComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::LifetimeComponent>(i)) {
        auto lifetimeComp = registry->getComponent<ecs::LifetimeComponent>(i);
        if (lifetimeComp) {
            data.push_back(static_cast<uint64_t>(LIFETIME));
            data.push_back(quantization::packTime(lifetimeComp->getLifetime()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertVelocityComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::VelocityComponent>(i)) {
        auto velocity = registry->getComponent<ecs::VelocityComponent>(i);
        if (velocity) {
            data.push_back(static_cast<uint64_t>(VELOCITY));
            data.push_back(quantization::packVelocity(velocity->getVelocity().getX()));
            data.push_back(quantization::packVelocity(velocity->getVelocity().getY()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertControllableTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ControllableTag>(i)) {
        data.push_back(static_cast<uint64_t>(CONTROLLABLE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertEnemyProjectileTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::EnnemyProjectileTag>(i)) {
        data.push_back(static_cast<uint64_t>(ENEMY_PROJECTILE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertGameZoneColliderTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::GameZoneColliderTag>(i)) {
        data.push_back(static_cast<uint64_t>(GAME_ZONE_COLLIDER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertMobTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::MobTag>(i)) {
        data.push_back(static_cast<uint64_t>(MOB_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertObstacleTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ObstacleTag>(i)) {
        data.push_back(static_cast<uint64_t>(OBSTACLE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertPlayerProjectileTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::PlayerProjectileTag>(i)) {
        data.push_back(static_cast<uint64_t>(PLAYER_PROJECTILE_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertShooterTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ShooterTag>(i)) {
        data.push_back(static_cast<uint64_t>(SHOOTER_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertProjectilePassThroughTagComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ProjectilePassThroughTag>(i)) {
        data.push_back(static_cast<uint64_t>(PROJECTILE_PASS_THROUGH_TAG));
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertProjectilePrefabComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ProjectilePrefabComponent>(i)) {
        auto prefabComp = registry->getComponent<ecs::ProjectilePrefabComponent>(i);
        if (prefabComp) {
            data.push_back(static_cast<uint64_t>(PROJECTILE_PREFAB));
            std::string prefabName = prefabComp->getPrefabName();
            for (char c : prefabName) {
                data.push_back(static_cast<uint64_t>(c));
            }
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ST));
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ND));
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_TRD));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertGameZoneComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::GameZoneComponent>(i)) {
        auto gameZoneComp = registry->getComponent<ecs::GameZoneComponent>(i);
        if (gameZoneComp) {
            data.push_back(static_cast<uint64_t>(GAME_ZONE));
            data.push_back(quantization::packSize(gameZoneComp->getZone().getHeight()));
            data.push_back(quantization::packSize(gameZoneComp->getZone().getWidth()));
            data.push_back(quantization::packPosition(gameZoneComp->getZone().getLeft()));
            data.push_back(quantization::packPosition(gameZoneComp->getZone().getTop()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertChargedShotComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i
) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::ChargedShotComponent>(i)) {
        auto chargedShotComp = registry->getComponent<ecs::ChargedShotComponent>(i);
        if (chargedShotComp) {
            data.push_back(static_cast<uint64_t>(CHARGED_SHOT_COMP));
            data.push_back(quantization::packDamage(chargedShotComp->getCharge()));
            data.push_back(quantization::packDamage(chargedShotComp->getMaxCharge()));
            data.push_back(quantization::packTime(chargedShotComp->getReloadTime()));
        }
    }
    return data;
}

std::vector<uint64_t> rserv::Lobby::convertAnimationStateComponent(
    std::shared_ptr<ecs::Registry> registry, ecs::Entity i) {
    std::vector<uint64_t> data;
    if (registry && registry->hasComponent<ecs::AnimationStateComponent>(i)) {
        auto animStateComp = registry->getComponent<ecs::AnimationStateComponent>(i);
        if (animStateComp) {
            data.push_back(static_cast<uint64_t>(ANIMATION_STATE));
            std::string state = animStateComp->getCurrentState();
            for (char c : state) {
                data.push_back(static_cast<uint64_t>(c));
            }
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ST));
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_ND));
            data.push_back(static_cast<uint64_t>(constants::END_OFSTRING_TRD));
            registry->removeOneComponent<ecs::AnimationStateComponent>(i);
        }
    }
    return data;
}
