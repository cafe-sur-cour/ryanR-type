/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientGameStateConversions Unit Tests
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include <string>
#include <cstring>

#include "../../client/ClientNetwork.hpp"
#include "../../common/interfaces/INetwork.hpp"
#include "../../common/interfaces/IBuffer.hpp"
#include "../../common/interfaces/IPacketManager.hpp"
#include "../../common/resourceManager/ResourceManager.hpp"
#include "../../common/gsm/IGameStateMachine.hpp"
#include "../../common/constants.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"
#include "../../common/components/permanent/TransformComponent.hpp"
#include "../../common/components/permanent/HealthComponent.hpp"
#include "../../common/components/permanent/ScoreComponent.hpp"
#include "../../common/components/permanent/VelocityComponent.hpp"
#include "../../common/components/permanent/ProjectilePrefabComponent.hpp"
#include "../../common/components/permanent/ChargedShotComponent.hpp"
#include "../../common/components/tags/ObstacleTag.hpp"
#include "../../client/components/permanent/NetworkStateComponent.hpp"

// Helper function to pack float as uint64_t
inline uint64_t packFloat(float value) {
    uint64_t bits;
    std::memcpy(&bits, &value, sizeof(float));
    return bits;
}

// Test fixture for ClientGameStateConversions
class ClientGameStateConversionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        _registry = std::make_shared<ecs::Registry>();
        _resourceManager = std::make_shared<ResourceManager>();
        _resourceManager->add<ecs::Registry>(_registry);

        _clientNetwork = std::make_shared<ClientNetwork>();
        _clientNetwork->setResourceManager(_resourceManager);
        _clientNetwork->setDebugMode(false);
    }

    std::shared_ptr<ecs::Registry> _registry;
    std::shared_ptr<ResourceManager> _resourceManager;
    std::shared_ptr<ClientNetwork> _clientNetwork;
    static constexpr ecs::Entity TEST_ENTITY_ID = 42;
    
    // Helper to check transform component values (accessing through getters)
    void assertTransformEquals(ecs::Entity entityId, float expectedX, float expectedY,
                              float expectedRotation, float expectedScaleX, float expectedScaleY) {
        auto transform = _registry->getComponent<ecs::TransformComponent>(entityId);
        EXPECT_FLOAT_EQ(transform->getPosition().getX(), expectedX);
        EXPECT_FLOAT_EQ(transform->getPosition().getY(), expectedY);
        EXPECT_FLOAT_EQ(transform->getRotation(), expectedRotation);
        EXPECT_FLOAT_EQ(transform->getScale().getX(), expectedScaleX);
        EXPECT_FLOAT_EQ(transform->getScale().getY(), expectedScaleY);
    }
    
    // Helper to check velocity component values
    void assertVelocityEquals(ecs::Entity entityId, float expectedX, float expectedY) {
        auto velocity = _registry->getComponent<ecs::VelocityComponent>(entityId);
        EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), expectedX);
        EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), expectedY);
    }
};

// ============================================================================
// Transform Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseTransformComponentCreatesNewComponent) {
    std::vector<uint64_t> payload = {
        packFloat(10.5f),  // posX
        packFloat(20.3f),  // posY
        packFloat(45.0f),  // rotation
        packFloat(2.0f),   // scaleX
        packFloat(1.5f)    // scaleY
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
    EXPECT_TRUE(_registry->hasComponent<ecs::TransformComponent>(TEST_ENTITY_ID));

    assertTransformEquals(TEST_ENTITY_ID, 10.5f, 20.3f, 45.0f, 2.0f, 1.5f);
}

TEST_F(ClientGameStateConversionsTest, ParseTransformComponentUpdatesExistingComponent) {
    // Create initial component
    auto initialTransform = std::make_shared<ecs::TransformComponent>(
        math::Vector2f(5.0f, 5.0f), 0.0f, math::Vector2f(1.0f, 1.0f)
    );
    _registry->addComponent(TEST_ENTITY_ID, initialTransform);

    std::vector<uint64_t> payload = {
        packFloat(15.5f),  // posX
        packFloat(25.3f),  // posY
        packFloat(90.0f),  // rotation
        packFloat(3.0f),   // scaleX
        packFloat(2.5f)    // scaleY
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
    assertTransformEquals(TEST_ENTITY_ID, 15.5f, 25.3f, 90.0f, 3.0f, 2.5f);
}

TEST_F(ClientGameStateConversionsTest, ParseTransformComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(10.5f),
        packFloat(20.3f)
        // Missing rotation, scaleX, scaleY
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_FALSE(_registry->hasComponent<ecs::TransformComponent>(TEST_ENTITY_ID));
}

TEST_F(ClientGameStateConversionsTest, ParseTransformComponentWithOffset) {
    std::vector<uint64_t> payload = {
        packFloat(1.0f),   // dummy
        packFloat(2.0f),   // dummy
        packFloat(30.0f),  // posX
        packFloat(40.0f),  // posY
        packFloat(15.0f),  // rotation
        packFloat(1.2f),   // scaleX
        packFloat(0.8f)    // scaleY
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 2, TEST_ENTITY_ID);

    EXPECT_EQ(result, 7);
    assertTransformEquals(TEST_ENTITY_ID, 30.0f, 40.0f, 15.0f, 1.2f, 0.8f);
}

// ============================================================================
// Health Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseHealthComponentCreatesNewComponent) {
    std::vector<uint64_t> payload = {
        packFloat(75.0f),   // health
        packFloat(100.0f)   // baseHealth
    };

    size_t result = _clientNetwork->parseHealthComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 2);
    EXPECT_TRUE(_registry->hasComponent<ecs::HealthComponent>(TEST_ENTITY_ID));

    auto health = _registry->getComponent<ecs::HealthComponent>(TEST_ENTITY_ID);
    EXPECT_FLOAT_EQ(health->getHealth(), 75.0f);
    EXPECT_FLOAT_EQ(health->getBaseHealth(), 100.0f);
}

TEST_F(ClientGameStateConversionsTest, ParseHealthComponentUpdatesExistingComponent) {
    auto initialHealth = std::make_shared<ecs::HealthComponent>(50.0f);
    initialHealth->setBaseHealth(100.0f);
    _registry->addComponent(TEST_ENTITY_ID, initialHealth);

    std::vector<uint64_t> payload = {
        packFloat(25.0f),   // health
        packFloat(100.0f)   // baseHealth
    };

    size_t result = _clientNetwork->parseHealthComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 2);
    auto health = _registry->getComponent<ecs::HealthComponent>(TEST_ENTITY_ID);
    EXPECT_FLOAT_EQ(health->getHealth(), 25.0f);
    EXPECT_FLOAT_EQ(health->getBaseHealth(), 100.0f);
}

TEST_F(ClientGameStateConversionsTest, ParseHealthComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(75.0f)
        // Missing baseHealth
    };

    size_t result = _clientNetwork->parseHealthComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_FALSE(_registry->hasComponent<ecs::HealthComponent>(TEST_ENTITY_ID));
}

// ============================================================================
// Velocity Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseVelocityComponentCreatesNewComponent) {
    std::vector<uint64_t> payload = {
        packFloat(5.5f),    // velX
        packFloat(-3.2f)    // velY
    };

    size_t result = _clientNetwork->parseVelocityComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 2);
    EXPECT_TRUE(_registry->hasComponent<ecs::VelocityComponent>(TEST_ENTITY_ID));

    assertVelocityEquals(TEST_ENTITY_ID, 5.5f, -3.2f);
}

TEST_F(ClientGameStateConversionsTest, ParseVelocityComponentUpdatesExistingComponent) {
    auto initialVelocity = std::make_shared<ecs::VelocityComponent>(math::Vector2f(1.0f, 1.0f));
    _registry->addComponent(TEST_ENTITY_ID, initialVelocity);

    std::vector<uint64_t> payload = {
        packFloat(10.0f),   // velX
        packFloat(-5.0f)    // velY
    };

    size_t result = _clientNetwork->parseVelocityComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 2);
    assertVelocityEquals(TEST_ENTITY_ID, 10.0f, -5.0f);
}

TEST_F(ClientGameStateConversionsTest, ParseVelocityComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(5.5f)
        // Missing velY
    };

    size_t result = _clientNetwork->parseVelocityComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_FALSE(_registry->hasComponent<ecs::VelocityComponent>(TEST_ENTITY_ID));
}

// ============================================================================
// Score Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseScoreComponentCreatesNewComponent) {
    std::vector<uint64_t> payload = {
        static_cast<uint64_t>(1500)
    };

    size_t result = _clientNetwork->parseScoreComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
    EXPECT_TRUE(_registry->hasComponent<ecs::ScoreComponent>(TEST_ENTITY_ID));

    auto score = _registry->getComponent<ecs::ScoreComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(score->getScore(), 1500);
}

TEST_F(ClientGameStateConversionsTest, ParseScoreComponentUpdatesExistingComponent) {
    auto initialScore = std::make_shared<ecs::ScoreComponent>(500);
    _registry->addComponent(TEST_ENTITY_ID, initialScore);

    std::vector<uint64_t> payload = {
        static_cast<uint64_t>(2500)
    };

    size_t result = _clientNetwork->parseScoreComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
    auto score = _registry->getComponent<ecs::ScoreComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(score->getScore(), 2500);
}

TEST_F(ClientGameStateConversionsTest, ParseScoreComponentEmptyPayload) {
    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseScoreComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

// ============================================================================
// Obstacle Tag Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseObstacleTagComponentAddsTag) {
    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseObstacleTagComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_TRUE(_registry->hasComponent<ecs::ObstacleTag>(TEST_ENTITY_ID));
}

TEST_F(ClientGameStateConversionsTest, ParseObstacleTagComponentDoesNotDuplicateTag) {
    auto initialTag = std::make_shared<ecs::ObstacleTag>();
    _registry->addComponent(TEST_ENTITY_ID, initialTag);

    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseObstacleTagComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_TRUE(_registry->hasComponent<ecs::ObstacleTag>(TEST_ENTITY_ID));
}

// ============================================================================
// Projectile Prefab Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseProjectilePrefabComponentCreatesComponent) {
    std::string prefabName = "laser";
    std::vector<uint64_t> payload;

    for (char c : prefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>('\0'));

    size_t result = _clientNetwork->parseProjectilePrefabComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, prefabName.length() + 1);
    EXPECT_TRUE(_registry->hasComponent<ecs::ProjectilePrefabComponent>(TEST_ENTITY_ID));

    auto prefab = _registry->getComponent<ecs::ProjectilePrefabComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(prefab->getPrefabName(), prefabName);
}

TEST_F(ClientGameStateConversionsTest, ParseProjectilePrefabComponentWithCarriageReturn) {
    std::string prefabName = "missile";
    std::vector<uint64_t> payload;

    for (char c : prefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>('\r'));
    payload.push_back(static_cast<uint64_t>('\n'));

    size_t result = _clientNetwork->parseProjectilePrefabComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, prefabName.length() + 2);
    auto prefab = _registry->getComponent<ecs::ProjectilePrefabComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(prefab->getPrefabName(), prefabName);
}

TEST_F(ClientGameStateConversionsTest, ParseProjectilePrefabComponentUpdatesExisting) {
    auto initialPrefab = std::make_shared<ecs::ProjectilePrefabComponent>("old_prefab");
    _registry->addComponent(TEST_ENTITY_ID, initialPrefab);

    std::string newPrefabName = "new_prefab";
    std::vector<uint64_t> payload;

    for (char c : newPrefabName) {
        payload.push_back(static_cast<uint64_t>(c));
    }
    payload.push_back(static_cast<uint64_t>('\0'));

    _clientNetwork->parseProjectilePrefabComponent(payload, 0, TEST_ENTITY_ID);

    auto prefab = _registry->getComponent<ecs::ProjectilePrefabComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(prefab->getPrefabName(), newPrefabName);
}

// ============================================================================
// Charged Shot Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseChargedShotComponentCreatesComponent) {
    std::vector<uint64_t> payload = {
        packFloat(50.0f),   // charge
        packFloat(100.0f),  // maxCharge
        packFloat(1.5f)     // reloadTime
    };

    size_t result = _clientNetwork->parseChargedShotComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 3);
    EXPECT_TRUE(_registry->hasComponent<ecs::ChargedShotComponent>(TEST_ENTITY_ID));

    auto chargedShot = _registry->getComponent<ecs::ChargedShotComponent>(TEST_ENTITY_ID);
    EXPECT_FLOAT_EQ(chargedShot->getCharge(), 50.0f);
    EXPECT_FLOAT_EQ(chargedShot->getMaxCharge(), 100.0f);
    EXPECT_FLOAT_EQ(chargedShot->getReloadTime(), 1.5f);
}

TEST_F(ClientGameStateConversionsTest, ParseChargedShotComponentUpdatesExisting) {
    auto initialChargedShot = std::make_shared<ecs::ChargedShotComponent>(25.0f, 80.0f, 1.0f);
    _registry->addComponent(TEST_ENTITY_ID, initialChargedShot);

    std::vector<uint64_t> payload = {
        packFloat(75.0f),   // charge
        packFloat(120.0f),  // maxCharge
        packFloat(2.0f)     // reloadTime
    };

    size_t result = _clientNetwork->parseChargedShotComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 3);
    auto chargedShot = _registry->getComponent<ecs::ChargedShotComponent>(TEST_ENTITY_ID);
    EXPECT_FLOAT_EQ(chargedShot->getCharge(), 75.0f);
    EXPECT_FLOAT_EQ(chargedShot->getMaxCharge(), 120.0f);
    EXPECT_FLOAT_EQ(chargedShot->getReloadTime(), 2.0f);
}

TEST_F(ClientGameStateConversionsTest, ParseChargedShotComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(50.0f),
        packFloat(100.0f)
        // Missing reloadTime
    };

    size_t result = _clientNetwork->parseChargedShotComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
    EXPECT_FALSE(_registry->hasComponent<ecs::ChargedShotComponent>(TEST_ENTITY_ID));
}

// ============================================================================
// Tag Component Tests (Simple tag parsing)
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParsePlayerTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parsePlayerTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseControllableTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseControllableTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseEnemyProjectileTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseEnemyProjectileTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseGameZoneColliderTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseGameZoneColliderTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseMobTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseMobTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParsePlayerProjectileTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parsePlayerProjectileTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseScoreTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseScoreTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseShooterTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseShooterTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseProjectilePassThroughTagComponent) {
    std::vector<uint64_t> payload;
    size_t result = _clientNetwork->parseProjectilePassThroughTagComponent(payload, 0, TEST_ENTITY_ID);
    EXPECT_EQ(result, 0);
}

// ============================================================================
// Other Component Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, ParseSpeedComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        packFloat(15.5f)
    };

    size_t result = _clientNetwork->parseSpeedComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
}

TEST_F(ClientGameStateConversionsTest, ParseSpeedComponentInsufficientPayload) {
    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseSpeedComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseColliderComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        packFloat(1.0f),    // offsetX
        packFloat(2.0f),    // offsetY
        packFloat(10.0f),   // sizeX
        packFloat(20.0f),   // sizeY
        static_cast<uint64_t>(1) // colliderType
    };

    size_t result = _clientNetwork->parseColliderComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
}

TEST_F(ClientGameStateConversionsTest, ParseColliderComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(1.0f),
        packFloat(2.0f)
    };

    size_t result = _clientNetwork->parseColliderComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseShootingStatsComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        packFloat(0.5f),    // fireRate
        packFloat(1.0f),    // cooldown
        static_cast<uint64_t>(3),     // shotCount
        packFloat(15.0f),   // angleSpread
        packFloat(5.0f)     // offsetDistance
    };

    size_t result = _clientNetwork->parseShootingStatsComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
}

TEST_F(ClientGameStateConversionsTest, ParseShootingStatsComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(0.5f)
    };

    size_t result = _clientNetwork->parseShootingStatsComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseDamageComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        static_cast<uint64_t>(25)
    };

    size_t result = _clientNetwork->parseDamageComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
}

TEST_F(ClientGameStateConversionsTest, ParseDamageComponentEmptyPayload) {
    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseDamageComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseLifetimeComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        packFloat(5.0f)
    };

    size_t result = _clientNetwork->parseLifetimeComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
}

TEST_F(ClientGameStateConversionsTest, ParseLifetimeComponentEmptyPayload) {
    std::vector<uint64_t> payload;

    size_t result = _clientNetwork->parseLifetimeComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

TEST_F(ClientGameStateConversionsTest, ParseGameZoneComponentWithValidPayload) {
    std::vector<uint64_t> payload = {
        packFloat(600.0f),  // height
        packFloat(800.0f),  // width
        packFloat(0.0f),    // left
        packFloat(0.0f)     // top
    };

    size_t result = _clientNetwork->parseGameZoneComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 4);
}

TEST_F(ClientGameStateConversionsTest, ParseGameZoneComponentInsufficientPayload) {
    std::vector<uint64_t> payload = {
        packFloat(600.0f)
    };

    size_t result = _clientNetwork->parseGameZoneComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 0);
}

// ============================================================================
// Edge Cases and Integration Tests
// ============================================================================

TEST_F(ClientGameStateConversionsTest, MultipleComponentsOnSameEntity) {
    // Add Transform
    std::vector<uint64_t> transformPayload = {
        packFloat(10.0f), packFloat(20.0f), packFloat(0.0f),
        packFloat(1.0f), packFloat(1.0f)
    };
    _clientNetwork->parseTransformComponent(transformPayload, 0, TEST_ENTITY_ID);

    // Add Health
    std::vector<uint64_t> healthPayload = {
        packFloat(80.0f), packFloat(100.0f)
    };
    _clientNetwork->parseHealthComponent(healthPayload, 0, TEST_ENTITY_ID);

    // Add Velocity
    std::vector<uint64_t> velocityPayload = {
        packFloat(5.0f), packFloat(-2.0f)
    };
    _clientNetwork->parseVelocityComponent(velocityPayload, 0, TEST_ENTITY_ID);

    EXPECT_TRUE(_registry->hasComponent<ecs::TransformComponent>(TEST_ENTITY_ID));
    EXPECT_TRUE(_registry->hasComponent<ecs::HealthComponent>(TEST_ENTITY_ID));
    EXPECT_TRUE(_registry->hasComponent<ecs::VelocityComponent>(TEST_ENTITY_ID));
}

TEST_F(ClientGameStateConversionsTest, ZeroValuesInComponents) {
    std::vector<uint64_t> payload = {
        packFloat(0.0f), packFloat(0.0f), packFloat(0.0f),
        packFloat(0.0f), packFloat(0.0f)
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
    assertTransformEquals(TEST_ENTITY_ID, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

TEST_F(ClientGameStateConversionsTest, NegativeValuesInComponents) {
    std::vector<uint64_t> payload = {
        packFloat(-10.5f), packFloat(-20.3f), packFloat(-45.0f),
        packFloat(2.0f), packFloat(1.5f)
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
    assertTransformEquals(TEST_ENTITY_ID, -10.5f, -20.3f, -45.0f, 2.0f, 1.5f);
}

TEST_F(ClientGameStateConversionsTest, LargeValuesInComponents) {
    std::vector<uint64_t> payload = {
        packFloat(10000.5f), packFloat(20000.3f), packFloat(360.0f),
        packFloat(100.0f), packFloat(100.0f)
    };

    size_t result = _clientNetwork->parseTransformComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 5);
    assertTransformEquals(TEST_ENTITY_ID, 10000.5f, 20000.3f, 360.0f, 100.0f, 100.0f);
}

TEST_F(ClientGameStateConversionsTest, EmptyProjectilePrefabName) {
    std::vector<uint64_t> payload = {
        static_cast<uint64_t>('\0')
    };

    size_t result = _clientNetwork->parseProjectilePrefabComponent(payload, 0, TEST_ENTITY_ID);

    EXPECT_EQ(result, 1);
    auto prefab = _registry->getComponent<ecs::ProjectilePrefabComponent>(TEST_ENTITY_ID);
    EXPECT_EQ(prefab->getPrefabName(), "");
}

TEST_F(ClientGameStateConversionsTest, PayloadWithOffsetAndInsufficientData) {
    std::vector<uint64_t> payload = {
        packFloat(1.0f),
        packFloat(2.0f),
        packFloat(3.0f)
    };

    // Starting at offset 2, we only have 1 element left, need 5 for transform
    size_t result = _clientNetwork->parseTransformComponent(payload, 2, TEST_ENTITY_ID);

    EXPECT_EQ(result, 2);  // Should return original offset unchanged
}
