/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/ECS/system/movement/InputToVelocitySystem.hpp"
#include "../../../../common/ECS/system/movement/MovementSystem.hpp"
#include "../../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../../common/ECS/component/temporary/InputIntentComponent.hpp"
#include "../../../../common/ECS/component/permanent/VelocityComponent.hpp"
#include "../../../../common/ECS/component/permanent/TransformComponent.hpp"
#include "../../../../common/ECS/component/permanent/SpeedComponent.hpp"
#include "../../../../common/ECS/resourceManager/ResourceManager.hpp"

using namespace ecs;

/* MovementSystem Tests */

class MovementSystemsTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<ARegistry>();
        resourceManager = std::make_shared<ResourceManager>();
        inputToVelocitySystem = std::make_shared<InputToVelocitySystem>();
        movementSystem = std::make_shared<MovementSystem>();

        // Register components
        registry->registerComponent<InputIntentComponent>();
        registry->registerComponent<VelocityComponent>();
        registry->registerComponent<TransformComponent>();
        registry->registerComponent<SpeedComponent>();
    }

    std::shared_ptr<ARegistry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<InputToVelocitySystem> inputToVelocitySystem;
    std::shared_ptr<MovementSystem> movementSystem;
};

TEST_F(MovementSystemsTest, NoEntities_NoUpdate) {
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);
    movementSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(MovementSystemsTest, EntityWithIntentAndTransform_UsesBaseSpeed) {
    // Create entity with input intent and transform
    int entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Check velocity created: (1,0) * 300 = (300, 0)
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 300.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Intent should be processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);

    // Now apply movement
    float deltaTime = 0.016f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // Position should be updated: 0 + (300,0) * 0.016 = (4.8, 0)
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 4.8f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);
}

TEST_F(MovementSystemsTest, EntityWithIntentAndTransform_MovesVertically) {
    // Create entity with input intent and transform
    int entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 1.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(10.0f, 20.0f));

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.02f);

    // Check velocity: (0,1) * 300 = (0, 300)
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 300.0f);

    // Now apply movement
    float deltaTime = 0.02f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // Position: (10,20) + (0,300) * 0.02 = (10, 20 + 6) = (10, 26)
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 10.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 26.0f);

    // Intent processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);
}

TEST_F(MovementSystemsTest, ZeroDirectionIntent_NoMovement) {
    // Create entity with zero direction intent
    int entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 0.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be zero
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 0.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Apply movement
    movementSystem->update(resourceManager, registry, 0.016f);

    // Position should remain unchanged
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 0.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);

    // Intent processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);
}

TEST_F(MovementSystemsTest, EntityWithIntentWithoutTransform_CreatesVelocity) {
    // Entity with intent but no transform
    int entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));

    registry->addComponent<InputIntentComponent>(entityId, intent);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be created
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 300.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Intent processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);

    // Movement system does nothing since no Transform
    movementSystem->update(resourceManager, registry, 0.016f);
}

TEST_F(MovementSystemsTest, EntityWithTransformWithoutVelocity_NoUpdate) {
    // Entity with transform but no velocity
    int entityId = 0;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<TransformComponent>(entityId, transform);

    // No input to velocity processing

    // Movement system does nothing
    movementSystem->update(resourceManager, registry, 0.016f);

    // Position unchanged
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 0.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);
}

TEST_F(MovementSystemsTest, MultipleEntities_UpdatesCorrectly) {
    // Entity 0: with intent and transform
    int entityId0 = 0;
    auto intent0 = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto transform0 = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    registry->addComponent<InputIntentComponent>(entityId0, intent0);
    registry->addComponent<TransformComponent>(entityId0, transform0);

    // Entity 1: with intent and transform
    int entityId1 = 1;
    auto intent1 = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, -1.0f));
    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent<InputIntentComponent>(entityId1, intent1);
    registry->addComponent<TransformComponent>(entityId1, transform1);

    // Process inputs to velocities
    inputToVelocitySystem->update(resourceManager, registry, 0.01f);

    // Apply movement
    float deltaTime = 0.01f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // Entity 0: (0,0) + (300,0) * 0.01 = (3, 0)
    auto updatedTransform0 = registry->getComponent<TransformComponent>(entityId0);
    EXPECT_EQ(updatedTransform0->getPosition().getX(), 3.0f);
    EXPECT_EQ(updatedTransform0->getPosition().getY(), 0.0f);

    // Entity 1: (100,100) + (0,-300) * 0.01 = (100, 97)
    auto updatedTransform1 = registry->getComponent<TransformComponent>(entityId1);
    EXPECT_EQ(updatedTransform1->getPosition().getX(), 100.0f);
    EXPECT_EQ(updatedTransform1->getPosition().getY(), 97.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
