/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/InputIntentComponent.hpp"
#include "../../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../../common/components/tags/ObstacleTag.hpp"
#include "../../../../common/CollisionRules/CollisionRules.hpp"

using namespace ecs;

/* MovementSystem Tests */

class MovementSystemsTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        inputToVelocitySystem = std::make_shared<InputToVelocitySystem>();
        movementSystem = std::make_shared<MovementSystem>();

        // Register components
        registry->registerComponent<InputIntentComponent>();
        registry->registerComponent<VelocityComponent>();
        registry->registerComponent<TransformComponent>();
        registry->registerComponent<SpeedComponent>();
        registry->registerComponent<ColliderComponent>();
        registry->registerComponent<ObstacleTag>();

        // Initialize collision rules with rules allowing solid collisions for tests
        CollisionRulesData testData;
        testData.solidAllowRules = std::make_shared<std::vector<CollisionRule>>();
        testData.triggerAllowRules = std::make_shared<std::vector<CollisionRule>>();
        testData.pushAllowRules = std::make_shared<std::vector<CollisionRule>>();
        CollisionRule allowAllSolid;
        allowAllSolid.groupA = {};
        allowAllSolid.groupB = {};
        testData.solidAllowRules->push_back(allowAllSolid);
        CollisionRules::initWithData(testData);
    }

    std::shared_ptr<Registry> registry;
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
    // Create entity with input intent, transform and speed
    ecs::Entity entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Check velocity created: (1,0) * 300 = (300, 0)
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 300.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Intent should be processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);

    // Now apply movement
    float deltaTime = 0.016f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // Position should be updated: 0 + (300,0) * 0.016 = (4.8, 0)
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 4.8f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);
}

TEST_F(MovementSystemsTest, EntityWithIntentAndTransform_MovesVertically) {
    // Create entity with input intent, transform and speed
    ecs::Entity entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 1.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(10.0f, 20.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<SpeedComponent>(entityId, speed);

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
}

TEST_F(MovementSystemsTest, ZeroDirectionIntent_NoMovement) {
    // Create entity with zero direction intent
    ecs::Entity entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, 0.0f));
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<SpeedComponent>(entityId, speed);

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
}

TEST_F(MovementSystemsTest, EntityWithIntentWithoutTransform_CreatesVelocity) {
    // ecs::Entity with intent but no transform
    ecs::Entity entityId = 0;
    auto intent = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto speed = std::make_shared<SpeedComponent>(300.0f);

    registry->addComponent<InputIntentComponent>(entityId, intent);
    registry->addComponent<SpeedComponent>(entityId, speed);

    // Process input to velocity
    inputToVelocitySystem->update(resourceManager, registry, 0.016f);

    // Velocity should be created
    ASSERT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    auto velocity = registry->getComponent<VelocityComponent>(entityId);
    EXPECT_EQ(velocity->getVelocity().getX(), 300.0f);
    EXPECT_EQ(velocity->getVelocity().getY(), 0.0f);

    // Intent processed
    auto updatedIntent = registry->getComponent<InputIntentComponent>(entityId);

    // Movement system does nothing since no Transform
    movementSystem->update(resourceManager, registry, 0.016f);
}

TEST_F(MovementSystemsTest, EntityWithTransformWithoutVelocity_NoUpdate) {
    // ecs::Entity with transform but no velocity
    ecs::Entity entityId = 0;
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
    // ecs::Entity 0: with intent and transform
    ecs::Entity entityId0 = 0;
    auto intent0 = std::make_shared<InputIntentComponent>(math::Vector2f(1.0f, 0.0f));
    auto transform0 = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto speed0 = std::make_shared<SpeedComponent>(300.0f);
    registry->addComponent<InputIntentComponent>(entityId0, intent0);
    registry->addComponent<TransformComponent>(entityId0, transform0);
    registry->addComponent<SpeedComponent>(entityId0, speed0);

    // ecs::Entity 1: with intent and transform
    ecs::Entity entityId1 = 1;
    auto intent1 = std::make_shared<InputIntentComponent>(math::Vector2f(0.0f, -1.0f));
    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    auto speed1 = std::make_shared<SpeedComponent>(300.0f);
    registry->addComponent<InputIntentComponent>(entityId1, intent1);
    registry->addComponent<TransformComponent>(entityId1, transform1);
    registry->addComponent<SpeedComponent>(entityId1, speed1);

    // Process inputs to velocities
    inputToVelocitySystem->update(resourceManager, registry, 0.01f);

    // Apply movement
    float deltaTime = 0.01f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // ecs::Entity 0: (0,0) + (300,0) * 0.01 = (3, 0)
    auto updatedTransform0 = registry->getComponent<TransformComponent>(entityId0);
    EXPECT_EQ(updatedTransform0->getPosition().getX(), 3.0f);
    EXPECT_EQ(updatedTransform0->getPosition().getY(), 0.0f);

    // ecs::Entity 1: (100,100) + (0,-300) * 0.01 = (100, 97)
    auto updatedTransform1 = registry->getComponent<TransformComponent>(entityId1);
    EXPECT_EQ(updatedTransform1->getPosition().getX(), 100.0f);
    EXPECT_EQ(updatedTransform1->getPosition().getY(), 97.0f);
}

// Collision Tests

TEST_F(MovementSystemsTest, SolidCollision_BlocksMovement) {
    // Create moving entity with solid collider
    size_t movingEntityId = 0;
    auto movingTransform = std::make_shared<TransformComponent>(
        math::Vector2f(0.0f, 0.0f));
    auto movingVelocity = std::make_shared<VelocityComponent>(
        math::Vector2f(50.0f, 0.0f)); // Move right
    auto movingCollider = std::make_shared<ColliderComponent>(
        math::Vector2f(0.0f, 0.0f), math::Vector2f(10.0f, 10.0f),
        CollisionType::Solid);

    registry->addComponent<TransformComponent>(movingEntityId, movingTransform);
    registry->addComponent<VelocityComponent>(movingEntityId, movingVelocity);
    registry->addComponent<ColliderComponent>(movingEntityId, movingCollider);

    // Create static obstacle
    size_t obstacleEntityId = 1;
    auto obstacleTransform = std::make_shared<TransformComponent>(
        math::Vector2f(20.0f, 0.0f)); // Directly in path
    auto obstacleCollider = std::make_shared<ColliderComponent>(
        math::Vector2f(0.0f, 0.0f), math::Vector2f(10.0f, 10.0f),
        CollisionType::Solid);

    registry->addComponent<TransformComponent>(obstacleEntityId, obstacleTransform);
    registry->addComponent<ColliderComponent>(obstacleEntityId, obstacleCollider);

    // Apply movement
    float deltaTime = 0.5f; // Smaller delta time for collision detection
    movementSystem->update(resourceManager, registry, deltaTime);

    // ecs::Entity should be blocked and slide smoothly to closest valid position
    auto updatedTransform = registry->getComponent<TransformComponent>(movingEntityId);
    EXPECT_LT(updatedTransform->getPosition().getX(), 20.0f); // Should not reach obstacle
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f); // Y unchanged
}

TEST_F(MovementSystemsTest, SolidCollision_AllowsSliding) {
    // Create moving entity with solid collider
    size_t movingEntityId = 0;
    auto movingTransform = std::make_shared<TransformComponent>(
        math::Vector2f(0.0f, 0.0f));
    auto movingVelocity = std::make_shared<VelocityComponent>(
        math::Vector2f(50.0f, 50.0f)); // Move diagonally
    auto movingCollider = std::make_shared<ColliderComponent>(
        math::Vector2f(0.0f, 0.0f), math::Vector2f(10.0f, 10.0f),
        CollisionType::Solid);

    registry->addComponent<TransformComponent>(movingEntityId, movingTransform);
    registry->addComponent<VelocityComponent>(movingEntityId, movingVelocity);
    registry->addComponent<ColliderComponent>(movingEntityId, movingCollider);

    // Create wall blocking horizontal movement
    size_t wallEntityId = 1;
    auto wallTransform = std::make_shared<TransformComponent>(
        math::Vector2f(20.0f, 0.0f));
    auto wallCollider = std::make_shared<ColliderComponent>(
        math::Vector2f(0.0f, 0.0f), math::Vector2f(10.0f, 100.0f),
        CollisionType::Solid);

    registry->addComponent<TransformComponent>(wallEntityId, wallTransform);
    registry->addComponent<ColliderComponent>(wallEntityId, wallCollider);

    // Apply movement
    float deltaTime = 0.5f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // ecs::Entity should slide vertically but be blocked horizontally
    auto updatedTransform = registry->getComponent<TransformComponent>(movingEntityId);
    EXPECT_LT(updatedTransform->getPosition().getX(), 20.0f); // Blocked horizontally
    EXPECT_GT(updatedTransform->getPosition().getY(), 0.0f); // Can move vertically
}

TEST_F(MovementSystemsTest, NoCollision_WhenNoColliders) {
    // Create entity with velocity but no collider
    ecs::Entity entityId = 0;
    auto transform = std::make_shared<TransformComponent>(
        math::Vector2f(0.0f, 0.0f));
    auto velocity = std::make_shared<VelocityComponent>(
        math::Vector2f(50.0f, 50.0f));

    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<VelocityComponent>(entityId, velocity);

    // Apply movement
    float deltaTime = 1.0f;
    movementSystem->update(resourceManager, registry, deltaTime);

    // Should move freely
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 50.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 50.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
