/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputToVelocitySystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../../common/components/permanent/SpeedComponent.hpp"

using namespace ecs;

/* InputToVelocitySystem Tests */

class InputToVelocitySystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        inputToVelocitySystem = std::make_shared<InputToVelocitySystem>();

        // Register components
        registry->registerComponent<InputIntentComponent>();
        registry->registerComponent<VelocityComponent>();
        registry->registerComponent<SpeedComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<InputToVelocitySystem> inputToVelocitySystem;
};

TEST_F(InputToVelocitySystemTest, NoEntities_NoUpdate) {
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(InputToVelocitySystemTest, EntityWithoutSpeedComponent_NoVelocityCreated) {
    // Create entity with input intent only, no speed
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // No velocity should be created
    EXPECT_FALSE(registry->hasComponent<VelocityComponent>(entityId));
}

TEST_F(InputToVelocitySystemTest, EntityWithInputAndSpeed_CreatesVelocity) {
    // Create entity with input intent and speed
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be created: direction (1,0) * speed 300 = (300, 0)
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 300.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Input intent should be removed
    EXPECT_FALSE(registry->hasComponent<InputIntentComponent>(entityId));
}

TEST_F(InputToVelocitySystemTest, EntityWithExistingVelocity_UpdatesVelocity) {
    // Create entity with input intent, speed, and existing velocity
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 1.0f));
    auto speed = std::make_shared<SpeedComponent>(200.0f);
    auto existingVelocity = std::make_shared<VelocityComponent>(math::Vector2f(100.0f, 0.0f));

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);
    registry->addComponent<VelocityComponent>(entityId, existingVelocity);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be updated: direction (0,1) * speed 200 = (0, 200)
    auto updatedVelocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedVelocity->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(updatedVelocity->getVelocity().getY(), 200.0f);

    // Input intent should be removed
    EXPECT_FALSE(registry->hasComponent<InputIntentComponent>(entityId));
}

TEST_F(InputToVelocitySystemTest, DiagonalInput_CorrectVelocity) {
    // Create entity with diagonal input
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 1.0f));
    auto speed = std::make_shared<SpeedComponent>(100.0f);

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be direction (1,1) * speed 100 = (100, 100)
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 100.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 100.0f);
}

TEST_F(InputToVelocitySystemTest, ZeroInput_ZeroVelocity) {
    // Create entity with zero input
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 0.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be zero
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 0.0f);
}

TEST_F(InputToVelocitySystemTest, NegativeInput_NegativeVelocity) {
    // Create entity with negative input
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(-1.0f, -0.5f));
    auto speed = std::make_shared<SpeedComponent>(200.0f);

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be negative
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), -200.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), -100.0f);
}

TEST_F(InputToVelocitySystemTest, MultipleEntities_AllProcessed) {
    // Create multiple entities with input and speed
    ecs::Entity entity1Id = 0;
    auto inputIntent1 = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto speed1 = std::make_shared<SpeedComponent>(100.0f);
    registry->addComponent<InputIntentComponent>(entity1Id, inputIntent1);
    registry->addComponent<SpeedComponent>(entity1Id, speed1);

    ecs::Entity entity2Id = 1;
    auto inputIntent2 = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 1.0f));
    auto speed2 = std::make_shared<SpeedComponent>(200.0f);
    registry->addComponent<InputIntentComponent>(entity2Id, inputIntent2);
    registry->addComponent<SpeedComponent>(entity2Id, speed2);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Check both entities processed
    auto velocity1 = registry->getComponent<VelocityComponent>(entity1Id);
    EXPECT_FLOAT_EQ(velocity1->getVelocity().getX(), 100.0f);
    EXPECT_FLOAT_EQ(velocity1->getVelocity().getY(), 0.0f);

    auto velocity2 = registry->getComponent<VelocityComponent>(entity2Id);
    EXPECT_FLOAT_EQ(velocity2->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(velocity2->getVelocity().getY(), 200.0f);

    // Both input intents should be removed
    EXPECT_FALSE(registry->hasComponent<InputIntentComponent>(entity1Id));
    EXPECT_FALSE(registry->hasComponent<InputIntentComponent>(entity2Id));
}

TEST_F(InputToVelocitySystemTest, ZeroSpeed_ZeroVelocity) {
    // Create entity with input but zero speed
    ecs::Entity entityId = 0;
    auto inputIntent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 1.0f));
    auto speed = std::make_shared<SpeedComponent>(0.0f);

    registry->addComponent<InputIntentComponent>(entityId, inputIntent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Update
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be zero
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_FLOAT_EQ(velocity->getVelocity().getY(), 0.0f);
}
