/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneStopSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/bounds/GameZoneStopSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../../common/components/permanent/GameZoneComponent.hpp"
#include "../../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../common/components/tags/GameZoneStopTag.hpp"

using namespace ecs;

class GameZoneStopSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        gameZoneStopSystem = std::make_shared<GameZoneStopSystem>();

        // Register components
        registry->registerComponent<PlayerTag>();
        registry->registerComponent<GameZoneStopTag>();
        registry->registerComponent<TransformComponent>();
        registry->registerComponent<VelocityComponent>();
        registry->registerComponent<GameZoneComponent>();
        registry->registerComponent<ColliderComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<GameZoneStopSystem> gameZoneStopSystem;
};

TEST_F(GameZoneStopSystemTest, NoEntities_NoUpdate) {
    gameZoneStopSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(GameZoneStopSystemTest, NoPlayer_NoUpdate) {
    // Create a stop zone without player
    ecs::Entity stopZoneId = 0;
    auto stopZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    auto stopZoneCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(50.0f, 50.0f));

    registry->addComponent<GameZoneStopTag>(stopZoneId, std::make_shared<GameZoneStopTag>());
    registry->addComponent<TransformComponent>(stopZoneId, stopZoneTransform);
    registry->addComponent<ColliderComponent>(stopZoneId, stopZoneCollider);

    // Update
    gameZoneStopSystem->update(resourceManager, registry, 0.016f);

    // Stop zone should still exist
    EXPECT_TRUE(registry->hasComponent<GameZoneStopTag>(stopZoneId));
}

TEST_F(GameZoneStopSystemTest, PlayerNotIntersecting_NoEffect) {
    // Create player
    ecs::Entity playerId = 0;
    auto playerTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto playerCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(32.0f, 32.0f));

    registry->addComponent<PlayerTag>(playerId, std::make_shared<PlayerTag>());
    registry->addComponent<TransformComponent>(playerId, playerTransform);
    registry->addComponent<ColliderComponent>(playerId, playerCollider);

    // Create stop zone far from player
    ecs::Entity stopZoneId = 1;
    auto stopZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(500.0f, 500.0f));
    auto stopZoneCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(50.0f, 50.0f));

    registry->addComponent<GameZoneStopTag>(stopZoneId, std::make_shared<GameZoneStopTag>());
    registry->addComponent<TransformComponent>(stopZoneId, stopZoneTransform);
    registry->addComponent<ColliderComponent>(stopZoneId, stopZoneCollider);

    // Create game zone with velocity
    ecs::Entity gameZoneId = 2;
    auto gameZoneVelocity = std::make_shared<VelocityComponent>(math::Vector2f(100.0f, 0.0f));

    registry->addComponent<GameZoneComponent>(gameZoneId, std::make_shared<GameZoneComponent>());
    registry->addComponent<VelocityComponent>(gameZoneId, gameZoneVelocity);

    // Update
    gameZoneStopSystem->update(resourceManager, registry, 0.016f);

    // Stop zone should still exist
    EXPECT_TRUE(registry->hasComponent<GameZoneStopTag>(stopZoneId));

    // Game zone velocity should not be changed
    auto velocity = registry->getComponent<VelocityComponent>(gameZoneId);
    EXPECT_EQ(velocity->getVelocity().getX(), 100.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);
}

TEST_F(GameZoneStopSystemTest, PlayerIntersecting_StopsGameZone) {
    // Create player
    ecs::Entity playerId = 0;
    auto playerTransform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    auto playerCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(32.0f, 32.0f));

    registry->addComponent<PlayerTag>(playerId, std::make_shared<PlayerTag>());
    registry->addComponent<TransformComponent>(playerId, playerTransform);
    registry->addComponent<ColliderComponent>(playerId, playerCollider);

    // Create stop zone intersecting with player
    ecs::Entity stopZoneId = 1;
    auto stopZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(110.0f, 110.0f));
    auto stopZoneCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(50.0f, 50.0f));

    registry->addComponent<GameZoneStopTag>(stopZoneId, std::make_shared<GameZoneStopTag>());
    registry->addComponent<TransformComponent>(stopZoneId, stopZoneTransform);
    registry->addComponent<ColliderComponent>(stopZoneId, stopZoneCollider);

    // Create game zone with velocity
    ecs::Entity gameZoneId = 2;
    auto gameZoneVelocity = std::make_shared<VelocityComponent>(math::Vector2f(100.0f, 50.0f));

    registry->addComponent<GameZoneComponent>(gameZoneId, std::make_shared<GameZoneComponent>());
    registry->addComponent<VelocityComponent>(gameZoneId, gameZoneVelocity);

    // Update
    gameZoneStopSystem->update(resourceManager, registry, 0.016f);

    // Stop zone should be destroyed
    EXPECT_FALSE(registry->hasComponent<GameZoneStopTag>(stopZoneId));

    // Game zone velocity should be set to zero
    auto velocity = registry->getComponent<VelocityComponent>(gameZoneId);
    EXPECT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);
}

TEST_F(GameZoneStopSystemTest, MultipleGameZones_AllStopped) {
    // Create player
    ecs::Entity playerId = 0;
    auto playerTransform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    auto playerCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(32.0f, 32.0f));

    registry->addComponent<PlayerTag>(playerId, std::make_shared<PlayerTag>());
    registry->addComponent<TransformComponent>(playerId, playerTransform);
    registry->addComponent<ColliderComponent>(playerId, playerCollider);

    // Create stop zone intersecting with player
    ecs::Entity stopZoneId = 1;
    auto stopZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(110.0f, 110.0f));
    auto stopZoneCollider = std::make_shared<ColliderComponent>(math::Vector2f(0.0f, 0.0f), math::Vector2f(50.0f, 50.0f));

    registry->addComponent<GameZoneStopTag>(stopZoneId, std::make_shared<GameZoneStopTag>());
    registry->addComponent<TransformComponent>(stopZoneId, stopZoneTransform);
    registry->addComponent<ColliderComponent>(stopZoneId, stopZoneCollider);

    // Create multiple game zones with velocity
    ecs::Entity gameZone1Id = 2;
    auto gameZone1Velocity = std::make_shared<VelocityComponent>(math::Vector2f(100.0f, 0.0f));
    registry->addComponent<GameZoneComponent>(gameZone1Id, std::make_shared<GameZoneComponent>());
    registry->addComponent<VelocityComponent>(gameZone1Id, gameZone1Velocity);

    ecs::Entity gameZone2Id = 3;
    auto gameZone2Velocity = std::make_shared<VelocityComponent>(math::Vector2f(50.0f, 25.0f));
    registry->addComponent<GameZoneComponent>(gameZone2Id, std::make_shared<GameZoneComponent>());
    registry->addComponent<VelocityComponent>(gameZone2Id, gameZone2Velocity);

    // Update
    gameZoneStopSystem->update(resourceManager, registry, 0.016f);

    // All game zones should have velocity set to zero
    auto velocity1 = registry->getComponent<VelocityComponent>(gameZone1Id);
    EXPECT_EQ(velocity1->getVelocity().getX(), 0.0f);
    EXPECT_EQ(velocity1->getVelocity().getY(), 0.0f);

    auto velocity2 = registry->getComponent<VelocityComponent>(gameZone2Id);
    EXPECT_EQ(velocity2->getVelocity().getX(), 0.0f);
    EXPECT_EQ(velocity2->getVelocity().getY(), 0.0f);
}
