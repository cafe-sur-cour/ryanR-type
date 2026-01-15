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
