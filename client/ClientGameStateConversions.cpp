/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client
*/

#include <vector>
#include <memory>
#include <string>
#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/components/permanent/NetworkIdComponent.hpp"
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/HealthComponent.hpp"
#include "../common/components/permanent/VelocityComponent.hpp"
#include "../common/components/tags/ObstacleTag.hpp"
#include "interpolation/NetworkStateComponent.hpp"

namespace {

inline float unpackFloat(uint64_t bits) {
    float value;
    memcpy(&value, &bits, sizeof(float));
    return value;
}

}

ecs::Entity ClientNetwork::findOrCreateNetworkEntity(std::shared_ptr<ecs::Registry> registry,
    size_t networkId) {
    for (ecs::Entity entity = 0; entity < registry->getMaxEntityId(); ++entity) {
        if (registry->hasComponent<ecs::NetworkIdComponent>(entity)) {
            auto netId = registry->getComponent<ecs::NetworkIdComponent>(entity);
            if (netId->getNetworkId() == networkId) {
                if (!registry->hasComponent<ecs::NetworkStateComponent>(entity)) {
                    auto networkState = std::make_shared<ecs::NetworkStateComponent>();
                    registry->addComponent(entity, networkState);
                    debug::Debug::printDebug(this->_isDebug,
                        "[CLIENT] Added NetworkStateComponent to existing entity " +
                        std::to_string(entity),
                        debug::debugType::NETWORK,
                        debug::debugLevel::INFO);
                }
                return entity;
            }
        }
    }
    ecs::Entity newEntity = registry->createEntity();
    auto netIdComp = std::make_shared<ecs::NetworkIdComponent>(networkId);
    registry->addComponent(newEntity, netIdComp);
    auto networkState = std::make_shared<ecs::NetworkStateComponent>();
    registry->addComponent(newEntity, networkState);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Created new entity " + std::to_string(newEntity) +
        " for network ID " + std::to_string(networkId),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return newEntity;
}

size_t ClientNetwork::parsePlayerTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has PlayerTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseTransformComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 5 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    float posX = unpackFloat(payload[index++]);
    float posY = unpackFloat(payload[index++]);
    float rotation = unpackFloat(payload[index++]);
    float scaleX = unpackFloat(payload[index++]);
    float scaleY = unpackFloat(payload[index++]);
    if (!registry->hasComponent<ecs::TransformComponent>(entityId)) {
        auto transform = std::make_shared<ecs::TransformComponent>();
        registry->addComponent(entityId, transform);
    } else {
        auto transform = registry->getComponent<ecs::TransformComponent>(entityId);
        transform->setPosition(math::Vector2f(posX, posY));
        transform->setRotation(rotation);
        transform->setScale(math::Vector2f(scaleX, scaleY));
    }
    if (registry->hasComponent<ecs::NetworkStateComponent>(entityId)) {
        auto networkState = registry->getComponent<ecs::NetworkStateComponent>(entityId);
        networkState->setCurrentTransform(
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

size_t ClientNetwork::parseSpeedComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 1 > payload.size()) return index;
    float speed = unpackFloat(payload[index++]);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Speed: " + std::to_string(speed),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseHealthComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 2 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    float health = unpackFloat(payload[index++]);
    float baseHealth = unpackFloat(payload[index++]);

    if (!registry->hasComponent<ecs::HealthComponent>(entityId)) {
        auto healthComp = std::make_shared<ecs::HealthComponent>(static_cast<float>(health));
        healthComp->setBaseHealth(static_cast<float>(baseHealth));
        registry->addComponent(entityId, healthComp);
    } else {
        auto healthComp = registry->getComponent<ecs::HealthComponent>(entityId);
        healthComp->setHealth(static_cast<float>(health));
        healthComp->setBaseHealth(static_cast<float>(baseHealth));
    }
    if (registry->hasComponent<ecs::NetworkStateComponent>(entityId)) {
        auto networkState = registry->getComponent<ecs::NetworkStateComponent>(entityId);
        networkState->setCurrentHealth(static_cast<uint32_t>(health),
            static_cast<uint32_t>(baseHealth));
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Health: " +
        std::to_string(health) + "/" + std::to_string(baseHealth),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseColliderComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 5 <= payload.size()) {
        float offsetX = unpackFloat(payload[index++]);
        float offsetY = unpackFloat(payload[index++]);
        float sizeX = unpackFloat(payload[index++]);
        float sizeY = unpackFloat(payload[index++]);
        uint8_t colliderType = static_cast<uint8_t>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Collider: offset(" +
            std::to_string(offsetX) + "," + std::to_string(offsetY) + ") size(" +
            std::to_string(sizeX) + "," + std::to_string(sizeY) + ") type(" +
            std::to_string(colliderType) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseShootingStatsComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 5 <= payload.size()) {
        float fireRate = unpackFloat(payload[index++]);
        float cooldown = unpackFloat(payload[index++]);
        uint32_t shotCount = static_cast<uint32_t>(payload[index++]);
        float angleSpread = unpackFloat(payload[index++]);
        float offsetDistance = unpackFloat(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " ShootingStats: fireRate(" +
            std::to_string(fireRate) + ") cooldown(" + std::to_string(cooldown) +
            ") shots(" + std::to_string(shotCount) + ") angleSpread(" +
            std::to_string(angleSpread) + ") offsetDistance(" +
            std::to_string(offsetDistance) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseScoreComponent(const std::vector<uint64_t> &payload, size_t index,
    ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        uint32_t score = static_cast<uint32_t>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Score: " + std::to_string(score),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseAIMovementPatternComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 6 <= payload.size()) {
        uint8_t pattern = static_cast<uint8_t>(payload[index++]);
        float amplitude = unpackFloat(payload[index++]);
        float frequency = unpackFloat(payload[index++]);
        float detectionRange = unpackFloat(payload[index++]);
        float deadzone = unpackFloat(payload[index++]);
        float timer = unpackFloat(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " AIMovementPattern: pattern(" +
            std::to_string(pattern) + ") amplitude(" + std::to_string(amplitude) +
            ") frequency(" + std::to_string(frequency) + ") detectionRange(" +
            std::to_string(detectionRange) + ") deadzone(" + std::to_string(deadzone) +
            ") timer(" + std::to_string(timer) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseDamageComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        uint32_t damage = static_cast<uint32_t>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Damage: " +
                std::to_string(damage),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseLifetimeComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        float lifetime = unpackFloat(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Lifetime: " +
                std::to_string(lifetime),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseVelocityComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 2 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    float velX = unpackFloat(payload[index++]);
    float velY = unpackFloat(payload[index++]);
    if (!registry->hasComponent<ecs::VelocityComponent>(entityId)) {
        auto velocity = std::make_shared<ecs::VelocityComponent>(math::Vector2f(velX, velY));
        registry->addComponent(entityId, velocity);
    } else {
        auto velocity = registry->getComponent<ecs::VelocityComponent>(entityId);
        velocity->setVelocity(math::Vector2f(velX, velY));
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Velocity: (" +
        std::to_string(velX) + "," + std::to_string(velY) + ")",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseAIMoverTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has AIMoverTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseAIShooterTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has AIShooterTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseControllableTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ControllableTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseEnemyProjectileTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has EnnemyProjectileTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseGameZoneColliderTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has GameZoneColliderTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseMobTagComponent(const std::vector<uint64_t> &payload, size_t index,
    ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has MobTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseObstacleTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    auto registry = this->_resourceManager->get<ecs::Registry>();

    if (!registry->hasComponent<ecs::ObstacleTag>(entityId)) {
        auto obstacleTag = std::make_shared<ecs::ObstacleTag>();
        registry->addComponent(entityId, obstacleTag);
    }

    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ObstacleTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parsePlayerProjectileTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has PlayerProjectileTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseScoreTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ScoreTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseShooterTagComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ShooterTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseProjectilePassThroughTagComponent(
    const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has ProjectilePassThroughTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseProjectilePrefabComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    std::string prefabName;
    while (index < payload.size()) {
        uint64_t charVal = payload[index++];
        if (charVal == static_cast<uint64_t>('\r')) {
            if (index < payload.size() && payload[index] == static_cast<uint64_t>('\n')) {
                index++;
            }
            break;
        }
        if (charVal == static_cast<uint64_t>('\0')) {
            break;
        }
        prefabName += static_cast<char>(charVal);
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " ProjectilePrefab: " + prefabName,
        debug::debugType::NETWORK, debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseNetworkIdComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 1 > payload.size()) return index;
    size_t networkId = static_cast<size_t>(payload[index++]);
    auto registry = this->_resourceManager->get<ecs::Registry>();
    if (!registry->hasComponent<ecs::NetworkIdComponent>(entityId)) {
        auto netIdComp = std::make_shared<ecs::NetworkIdComponent>(networkId);
        registry->addComponent(entityId, netIdComp);
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " NetworkId: " +
        std::to_string(networkId), debug::debugType::NETWORK, debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseGameZoneComponent(const std::vector<uint64_t> &payload,
    size_t index, ecs::Entity entityId) {
    if (index + 4 <= payload.size()) {
        float height = unpackFloat(payload[index++]);
        float width = unpackFloat(payload[index++]);
        float left = unpackFloat(payload[index++]);
        float top = unpackFloat(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " GameZone: height(" +
            std::to_string(height) + ") width(" + std::to_string(width) + ") left(" +
            std::to_string(left) + ") top(" + std::to_string(top) + ")",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}
