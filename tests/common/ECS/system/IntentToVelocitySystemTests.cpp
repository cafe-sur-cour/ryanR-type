/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IntentToVelocitySystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/movement/IntentToVelocitySystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/MovementIntentComponent.hpp"
#include "../../../../common/components/permanent/VelocityComponent.hpp"

using namespace ecs;

/* IntentToVelocitySystem Tests */

class IntentToVelocitySystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        intentToVelocitySystem = std::make_shared<IntentToVelocitySystem>();

        // Register components
        registry->registerComponent<MovementIntentComponent>();
        registry->registerComponent<VelocityComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<IntentToVelocitySystem> intentToVelocitySystem;
};

TEST_F(IntentToVelocitySystemTest, NoEntities_NoUpdate) {
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(IntentToVelocitySystemTest, EntityWithInactiveIntent_NoVelocityCreated) {
    // Create entity with inactive movement intent
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(1.0f, 0.0f), false);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // No velocity should be created
    EXPECT_FALSE(registry->hasComponent<VelocityComponent>(entityId));

    // Intent should still be present (not removed because inactive)
    EXPECT_TRUE(registry->hasComponent<MovementIntentComponent>(entityId));
}

TEST_F(IntentToVelocitySystemTest, EntityWithActiveIntent_CreatesVelocity) {
    // Create entity with active movement intent
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(100.0f, 50.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be created with the direction as velocity
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 100.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 50.0f);

    // Movement intent should be removed
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entityId));
}

TEST_F(IntentToVelocitySystemTest, EntityWithExistingVelocity_UpdatesVelocity) {
    // Create entity with active movement intent and existing velocity
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(200.0f, 100.0f), true);
    auto existingVelocity = std::make_shared<VelocityComponent>(math::Vector2f(50.0f, 25.0f));

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);
    registry->addComponent<VelocityComponent>(entityId, existingVelocity);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be updated to new direction
    auto updatedVelocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedVelocity->getVelocity().getX(), 200.0f);
    EXPECT_FLOAT_EQ(updatedVelocity->getVelocity().getY(), 100.0f);

    // Movement intent should be removed
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entityId));
}

TEST_F(IntentToVelocitySystemTest, ZeroDirection_ZeroVelocity) {
    // Create entity with active intent but zero direction
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(0.0f, 0.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be zero
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Movement intent should be removed
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entityId));
}

TEST_F(IntentToVelocitySystemTest, NegativeDirection_NegativeVelocity) {
    // Create entity with negative direction
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(-150.0f, -75.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be negative
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), -150.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), -75.0f);
}

TEST_F(IntentToVelocitySystemTest, MultipleActiveEntities_AllProcessed) {
    // Create multiple entities with active intents
    ecs::Entity entity1Id = 0;
    auto movementIntent1 = std::make_shared<MovementIntentComponent>(math::Vector2f(100.0f, 0.0f), true);
    registry->addComponent<MovementIntentComponent>(entity1Id, movementIntent1);

    ecs::Entity entity2Id = 1;
    auto movementIntent2 = std::make_shared<MovementIntentComponent>(math::Vector2f(0.0f, 200.0f), true);
    registry->addComponent<MovementIntentComponent>(entity2Id, movementIntent2);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Check both entities processed
    auto velocity1 = registry->getComponent<VelocityComponent>(entity1Id);
    EXPECT_FLOAT_EQ(velocity1->getVelocity().getX(), 100.0f);
    EXPECT_FLOAT_EQ(velocity1->getVelocity().getY(), 0.0f);

    auto velocity2 = registry->getComponent<VelocityComponent>(entity2Id);
    EXPECT_FLOAT_EQ(velocity2->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(velocity2->getVelocity().getY(), 200.0f);

    // Both movement intents should be removed
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entity1Id));
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entity2Id));
}

TEST_F(IntentToVelocitySystemTest, MixedActiveInactive_OnlyActiveProcessed) {
    // Create entities with mixed active/inactive intents
    ecs::Entity activeEntityId = 0;
    auto activeIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(100.0f, 100.0f), true);
    registry->addComponent<MovementIntentComponent>(activeEntityId, activeIntent);

    ecs::Entity inactiveEntityId = 1;
    auto inactiveIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(200.0f, 200.0f), false);
    registry->addComponent<MovementIntentComponent>(inactiveEntityId, inactiveIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Active entity should have velocity and intent removed
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(activeEntityId));
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(activeEntityId));

    // Inactive entity should not have velocity and intent should remain
    EXPECT_FALSE(registry->hasComponent<VelocityComponent>(inactiveEntityId));
    EXPECT_TRUE(registry->hasComponent<MovementIntentComponent>(inactiveEntityId));
}

TEST_F(IntentToVelocitySystemTest, LargeDirectionValues_HandledCorrectly) {
    // Create entity with large direction values
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(10000.0f, 5000.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // Update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should match the large values
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 10000.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 5000.0f);
}

TEST_F(IntentToVelocitySystemTest, RepeatedUpdates_ConsistentBehavior) {
    // Create entity with active intent
    ecs::Entity entityId = 0;
    auto movementIntent = std::make_shared<MovementIntentComponent>(math::Vector2f(100.0f, 50.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, movementIntent);

    // First update
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Verify velocity created and intent removed
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    EXPECT_FALSE(registry->hasComponent<MovementIntentComponent>(entityId));

    // Second update should not crash or cause issues
    intentToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should remain
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
}
