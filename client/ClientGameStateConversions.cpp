/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client Game State Conversions
*/

#include "ClientNetwork.hpp"
#include "../common/debug.hpp"
#include "../common/translationToECS.hpp"
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/components/permanent/NetworkIdComponent.hpp"
#include "../common/components/permanent/TransformComponent.hpp"
#include "../common/components/permanent/HealthComponent.hpp"
#include "../common/components/permanent/VelocityComponent.hpp"
#include "interpolation/NetworkStateComponent.hpp"

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

size_t ClientNetwork::parsePlayerTagComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    (void)payload;
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " has PlayerTag",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseTransformComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 5 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    float posX = static_cast<float>(payload[index++]);
    float posY = static_cast<float>(payload[index++]);
    float rotation = static_cast<float>(payload[index++]);
    float scaleX = static_cast<float>(payload[index++]);
    float scaleY = static_cast<float>(payload[index++]);
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

size_t ClientNetwork::parseSpeedComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 1 > payload.size()) return index;
    float speed = static_cast<float>(payload[index++]);
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Speed: " + std::to_string(speed),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseHealthComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 2 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    uint32_t health = static_cast<uint32_t>(payload[index++]);
    uint32_t baseHealth = static_cast<uint32_t>(payload[index++]);

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
        networkState->setCurrentHealth(health, baseHealth);
    }
    debug::Debug::printDebug(this->_isDebug,
        "[CLIENT] Entity " + std::to_string(entityId) + " Health: " +
        std::to_string(health) + "/" + std::to_string(baseHealth),
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
    return index;
}

size_t ClientNetwork::parseColliderComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 5 <= payload.size()) {
        float offsetX = static_cast<float>(payload[index++]);
        float offsetY = static_cast<float>(payload[index++]);
        float sizeX = static_cast<float>(payload[index++]);
        float sizeY = static_cast<float>(payload[index++]);
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

size_t ClientNetwork::parseShootingStatsComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 5 <= payload.size()) {
        float fireRate = static_cast<float>(payload[index++]);
        float cooldown = static_cast<float>(payload[index++]);
        uint32_t shotCount = static_cast<uint32_t>(payload[index++]);
        float angleSpread = static_cast<float>(payload[index++]);
        float offsetDistance = static_cast<float>(payload[index++]);
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

size_t ClientNetwork::parseScoreComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        uint32_t score = static_cast<uint32_t>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Score: " + std::to_string(score),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseAIMovementPatternComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 6 <= payload.size()) {
        uint8_t pattern = static_cast<uint8_t>(payload[index++]);
        float amplitude = static_cast<float>(payload[index++]);
        float frequency = static_cast<float>(payload[index++]);
        float detectionRange = static_cast<float>(payload[index++]);
        float deadzone = static_cast<float>(payload[index++]);
        float timer = static_cast<float>(payload[index++]);
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

size_t ClientNetwork::parseDamageComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        uint32_t damage = static_cast<uint32_t>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Damage: " + std::to_string(damage),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseLifetimeComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 1 <= payload.size()) {
        float lifetime = static_cast<float>(payload[index++]);
        debug::Debug::printDebug(this->_isDebug,
            "[CLIENT] Entity " + std::to_string(entityId) + " Lifetime: " + std::to_string(lifetime),
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    return index;
}

size_t ClientNetwork::parseVelocityComponent(const std::vector<uint64_t> &payload, size_t index, ecs::Entity entityId) {
    if (index + 2 > payload.size()) return index;
    auto registry = this->_resourceManager->get<ecs::Registry>();
    float velX = static_cast<float>(payload[index++]);
    float velY = static_cast<float>(payload[index++]);
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
