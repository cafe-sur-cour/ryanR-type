/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client
*/

#include <vector>
#include <memory>
#include <string>
#include <cstring>
#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/HealthComponent.hpp"
#include "../common/components/permanent/ScoreComponent.hpp"
#include "../common/components/permanent/VelocityComponent.hpp"
#include "../common/components/permanent/ProjectilePrefabComponent.hpp"
#include "../common/components/permanent/ChargedShotComponent.hpp"
#include "../common/components/tags/ObstacleTag.hpp"
#include "../common/components/permanent/AnimationStateComponent.hpp"
#include "../common/components/tags/LocalPlayerTag.hpp"
#include "../common/components/tags/PlayerTag.hpp"
#include "../common/FloatQuantization.hpp"
#include "components/permanent/NetworkStateComponent.hpp"

size_t ClientNetwork::parsePlayerTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has PlayerTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseTransformComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 5 > payload.size())
        return index;

    auto registry = this->_resourceManager->get<ecs::Registry>();

    float posX = quantization::unpackPosition(payload[index++]);
    float posY = quantization::unpackPosition(payload[index++]);
    float rotation = quantization::unpackRotation(payload[index++]);
    float scaleX = quantization::unpackScale(payload[index++]);
    float scaleY = quantization::unpackScale(payload[index++]);

    if (!registry->hasComponent<ecs::TransformComponent>(entityId))
        registry->addComponent(entityId, std::make_shared<ecs::TransformComponent>());

    if (!registry->hasComponent<ecs::LocalPlayerTag>(entityId)) {
        auto transform = registry->getComponent<ecs::TransformComponent>(entityId);
        transform->setPosition(math::Vector2f(posX, posY));
        transform->setRotation(rotation);
        transform->setScale(math::Vector2f(scaleX, scaleY));
    }

    if (registry->hasComponent<ecs::NetworkStateComponent>(entityId)) {
        registry->getComponent<ecs::NetworkStateComponent>(entityId)->setCurrentTransform(
            math::Vector2f(posX, posY),
            rotation,
            math::Vector2f(scaleX, scaleY)
        );
    }

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Transform: pos(" +
        std::to_string(posX) + "," + std::to_string(posY) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseSpeedComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 1 > payload.size())
        return index;

    float speed = quantization::unpackSpeed(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Speed: " + std::to_string(speed),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseHealthComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 2 > payload.size())
        return index;

    auto registry = this->_resourceManager->get<ecs::Registry>();

    float health = quantization::unpackHealth(payload[index++]);
    float baseHealth = quantization::unpackHealth(payload[index++]);

    if (!registry->hasComponent<ecs::HealthComponent>(entityId))
        registry->addComponent(entityId, std::make_shared<ecs::HealthComponent>());

    auto healthComp = registry->getComponent<ecs::HealthComponent>(entityId);
    healthComp->setHealth(static_cast<float>(health));
    healthComp->setBaseHealth(static_cast<float>(baseHealth));

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Health: " +
        std::to_string(health) + "/" + std::to_string(baseHealth),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseColliderComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 5 > payload.size())
        return index;

    float offsetX = quantization::unpackPosition(payload[index++]);
    float offsetY = quantization::unpackPosition(payload[index++]);
    float sizeX = quantization::unpackSize(payload[index++]);
    float sizeY = quantization::unpackSize(payload[index++]);

    uint8_t colliderType = static_cast<uint8_t>(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Collider: offset(" +
        std::to_string(offsetX) + "," + std::to_string(offsetY) + ") size(" +
        std::to_string(sizeX) + "," + std::to_string(sizeY) + ") type(" +
        std::to_string(colliderType) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseShootingStatsComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 6 > payload.size())
        return index;

    float fireRate = quantization::unpackTime(payload[index++]);
    float cooldown = quantization::unpackTime(payload[index++]);
    uint32_t shotCount = static_cast<uint32_t>(payload[index++]);
    float angleSpread = quantization::unpackAngle(payload[index++]);
    float offsetDistance = quantization::unpackSize(payload[index++]);
    float angleOffset = quantization::unpackAngle(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " ShootingStats: fireRate(" +
        std::to_string(fireRate) + ") cooldown(" + std::to_string(cooldown) +
        ") shots(" + std::to_string(shotCount) + ") angleSpread(" +
        std::to_string(angleSpread) + ") offsetDistance(" +
        std::to_string(offsetDistance) + ") angleOffset(" +
        std::to_string(angleOffset) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO
    );
    return index;
}

size_t ClientNetwork::parseScoreComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 1 > payload.size())
        return index;

    uint32_t score = static_cast<uint32_t>(payload[index++]);

    auto _registry = this->_resourceManager->get<ecs::Registry>();

    if (!_registry->hasComponent<ecs::ScoreComponent>(entityId))
        _registry->addComponent(entityId, std::make_shared<ecs::ScoreComponent>());

    _registry->getComponent<ecs::ScoreComponent>(entityId)->setScore(static_cast<int>(score));

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId)
            + " Score: " + std::to_string(score),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseDamageComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 1 > payload.size())
        return index;

    uint32_t damage = static_cast<uint32_t>(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Damage: " +
            std::to_string(damage),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseLifetimeComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 1 > payload.size())
        return index;

    float lifetime = quantization::unpackTime(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Lifetime: " +
            std::to_string(lifetime),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseVelocityComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 2 > payload.size())
        return index;

    auto registry = this->_resourceManager->get<ecs::Registry>();
    float velX = quantization::unpackVelocity(payload[index++]);
    float velY = quantization::unpackVelocity(payload[index++]);

    if (!registry->hasComponent<ecs::VelocityComponent>(entityId))
        registry->addComponent(entityId, std::make_shared<ecs::VelocityComponent>());

    auto velocity = registry->getComponent<ecs::VelocityComponent>(entityId);
    velocity->setVelocity(math::Vector2f(velX, velY));

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Velocity: (" +
        std::to_string(velX) + "," + std::to_string(velY) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}


size_t ClientNetwork::parseControllableTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ControllableTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseEnemyProjectileTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has EnnemyProjectileTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseGameZoneColliderTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has GameZoneColliderTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseMobTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has MobTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseObstacleTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (!registry->hasComponent<ecs::ObstacleTag>(entityId))
        registry->addComponent(entityId, std::make_shared<ecs::ObstacleTag>());

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ObstacleTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parsePlayerProjectileTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has PlayerProjectileTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseScoreTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ScoreTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseShooterTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ShooterTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseProjectilePassThroughTagComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    (void)payload;

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ProjectilePassThroughTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseProjectilePrefabComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    std::string prefabName;
    while (index < payload.size()) {
        uint64_t charVal = payload[index++];
        if (charVal == static_cast<uint64_t>(constants::END_OFSTRING_ST)) {
            if (index < payload.size() && payload[index] == static_cast<uint64_t>
                (constants::END_OFSTRING_ND)) {
                index++;
            }
            break;
        }
        if (charVal == static_cast<uint64_t>(constants::END_OFSTRING_TRD)) {
            break;
        }
        prefabName += static_cast<char>(charVal);
    }

    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (!registry->hasComponent<ecs::ProjectilePrefabComponent>(entityId)) {
        registry->addComponent(entityId, std::make_shared<ecs::ProjectilePrefabComponent>());
    }

    auto prefabComp = registry->getComponent<ecs::ProjectilePrefabComponent>(entityId);
    prefabComp->setPrefabName(prefabName);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " ProjectilePrefab: " + prefabName,
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseGameZoneComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 4 > payload.size())
        return index;

    float height = quantization::unpackSize(payload[index++]);
    float width = quantization::unpackSize(payload[index++]);
    float left = quantization::unpackPosition(payload[index++]);
    float top = quantization::unpackPosition(payload[index++]);

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " GameZone: height(" +
        std::to_string(height) + ") width(" + std::to_string(width) + ") left(" +
        std::to_string(left) + ") top(" + std::to_string(top) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseAnimationStateComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    std::string state;
    while (index < payload.size()) {
        uint64_t charVal = payload[index++];
        if (charVal == static_cast<uint64_t>(constants::END_OFSTRING_ST)) {
            if (index < payload.size() && payload[index] == static_cast<uint64_t>
                (constants::END_OFSTRING_ND)) {
                index++;
            }
            break;
        }
        if (charVal == static_cast<uint64_t>(constants::END_OFSTRING_TRD)) {
            break;
        }
        state += static_cast<char>(charVal);
    }

    auto it = _lastReceivedAnimationState.find(entityId);
    bool isNewState = (it == _lastReceivedAnimationState.end() || it->second != state);

    if (!state.empty()) {
        if (isNewState) {
            _lastReceivedAnimationState[entityId] = state;
        }

        auto registry = this->_resourceManager->get<ecs::Registry>();
        if (!registry->hasComponent<ecs::AnimationStateComponent>(entityId)) {
            registry->addComponent(entityId, std::make_shared<ecs::AnimationStateComponent>());
        }

        auto animStateComp = registry->getComponent<ecs::AnimationStateComponent>(entityId);
        if (animStateComp->getCurrentState().empty()) {
            animStateComp->setCurrentState(state);
        }
    }

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " AnimationState: " + state,
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseChargedShotComponent(
    const std::vector<uint64_t> &payload,
    size_t index,
    ecs::Entity entityId
) {
    if (index + 3 > payload.size())
        return index;

    float charge = quantization::unpackDamage(payload[index++]);
    float maxCharge = quantization::unpackDamage(payload[index++]);
    float reloadTime = quantization::unpackTime(payload[index++]);

    auto _registry = this->_resourceManager->get<ecs::Registry>();

    if (!_registry->hasComponent<ecs::ChargedShotComponent>(entityId)) {
        _registry->addComponent(entityId, std::make_shared<ecs::ChargedShotComponent>());
    }

    auto chargedShotComp = _registry->getComponent<ecs::ChargedShotComponent>(entityId);

    chargedShotComp->setCharge(charge);
    chargedShotComp->setMaxCharge(maxCharge);
    chargedShotComp->setReloadTime(reloadTime);

    debug::Debug::printDebug(
        this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId)
            + " Shot Charge: " + std::to_string(charge)
            + " Max Charge: " + std::to_string(maxCharge)
            + " Reload time: " + std::to_string(reloadTime),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO
    );
    return index;
}
