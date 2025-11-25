/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementInputSystem Tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "../../../common/ECS/system/input/MovementInputSystem.hpp"
#include "../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../common/ECS/component/tags/PlayerTag.hpp"
#include "../../../common/ECS/component/temporary/InputIntentComponent.hpp"
#include "../../../common/ECS/resourceManager/ResourceManager.hpp"

using namespace ecs;

/* MovementInputSystem Tests */

class MovementInputSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<ARegistry>();
        resourceManager = std::make_shared<ResourceManager>();
        system = std::make_shared<MovementInputSystem>();
    }

    std::shared_ptr<ARegistry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<MovementInputSystem> system;
};

TEST_F(MovementInputSystemTest, NoPlayerEntities_NoMovementIntent) {
    // No entities with PlayerTag
    system->updateSystem(resourceManager, registry, 0.016f);

    // Should not create any InputIntent components
    EXPECT_FALSE(registry->hasComponent<InputIntentComponent>(0));
}

TEST_F(MovementInputSystemTest, PlayerEntity_NoInput_NoMovementIntent) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // No input simulated
    system->updateSystem(resourceManager, registry, 0.016f);

    // Should create InputIntent with zero direction
    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getDirection().getX(), 0.0f);
    EXPECT_EQ(intent->getDirection().getY(), 0.0f);
}

TEST_F(MovementInputSystemTest, PlayerEntity_WithInput_CreatesMovementIntent) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // Simulate input
    system->simulateKeyPress(MovementKey::Right, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    // Should create InputIntent
    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getDirection().getX(), 1.0f);
    EXPECT_EQ(intent->getDirection().getY(), 0.0f);
}

TEST_F(MovementInputSystemTest, MultiplePlayerEntities_AllGetMovementIntent) {
    // Create multiple player entities
    registry->registerComponent<PlayerTag>();

    auto playerTag1 = std::make_shared<PlayerTag>();
    auto playerTag2 = std::make_shared<PlayerTag>();
    auto playerTag3 = std::make_shared<PlayerTag>();

    registry->addComponent(0, playerTag1);
    registry->addComponent(1, playerTag2);
    registry->addComponent(2, playerTag3);

    // Simulate input
    system->simulateKeyPress(MovementKey::Up, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    // All players should have InputIntent
    for (size_t i = 0; i < 3; ++i) {
        ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(i));
        auto intent = registry->getComponent<InputIntentComponent>(i);
        ASSERT_TRUE(intent);
        EXPECT_EQ(intent->getDirection().getX(), 0.0f);
        EXPECT_EQ(intent->getDirection().getY(), -1.0f);
    }
}

TEST_F(MovementInputSystemTest, DiagonalMovement_Normalized) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // Simulate diagonal input
    system->simulateKeyPress(MovementKey::Right, true);
    system->simulateKeyPress(MovementKey::Up, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    // Should create normalized diagonal InputIntent
    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);

    // Check normalization (length should be ~1.0)
    float length = std::sqrt(intent->getDirection().getX() * intent->getDirection().getX() +
                           intent->getDirection().getY() * intent->getDirection().getY());
    EXPECT_NEAR(length, 1.0f, 0.001f);
}

TEST_F(MovementInputSystemTest, InputReleased_IntentRemainsActive) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // First, create movement intent
    system->simulateKeyPress(MovementKey::Right, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getState(), ComponentState::Temporary);

    // Now release input - intent direction becomes zero
    system->simulateKeyPress(MovementKey::Right, false);
    system->updateSystem(resourceManager, registry, 0.016f);

    // Intent should be updated with zero direction and remain Temporary
    intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getDirection().getX(), 0.0f);
    EXPECT_EQ(intent->getDirection().getY(), 0.0f);
    EXPECT_EQ(intent->getState(), ComponentState::Temporary);
}

TEST_F(MovementInputSystemTest, AxisInput_OverridesKeyInput) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // Simulate both key and axis input
    system->simulateKeyPress(MovementKey::Right, true);
    system->simulateAxis(0.5f, 0.8f);  // Different direction
    system->updateSystem(resourceManager, registry, 0.016f);

    // Should use axis input, not key input
    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getDirection().getX(), 0.5f);
    EXPECT_EQ(intent->getDirection().getY(), 0.8f);
}

TEST_F(MovementInputSystemTest, ExistingIntent_UpdatedCorrectly) {
    // Create a player entity
    registry->registerComponent<PlayerTag>();
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // Create initial intent
    system->simulateKeyPress(MovementKey::Right, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    // Change input direction
    system->simulateKeyPress(MovementKey::Right, false);
    system->simulateKeyPress(MovementKey::Left, true);
    system->updateSystem(resourceManager, registry, 0.016f);

    // Intent should be updated
    ASSERT_TRUE(registry->hasComponent<InputIntentComponent>(0));
    auto intent = registry->getComponent<InputIntentComponent>(0);
    ASSERT_TRUE(intent);
    EXPECT_EQ(intent->getDirection().getX(), -1.0f);
    EXPECT_EQ(intent->getDirection().getY(), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}