/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/ECS/system/movement/MovementSystem.hpp"
#include "../../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../../common/ECS/component/temporary/MovementIntentComponent.hpp"
#include "../../../../common/ECS/component/permanent/TransformComponent.hpp"
#include "../../../../common/ECS/component/permanent/SpeedComponent.hpp"
#include "../../../../common/ECS/resourceManager/ResourceManager.hpp"

using namespace ecs;

/* MovementSystem Tests */

class MovementSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<ARegistry>();
        resourceManager = std::make_shared<ResourceManager>();
        system = std::make_shared<MovementSystem>();

        // Register components
        registry->registerComponent<MovementIntentComponent>();
        registry->registerComponent<TransformComponent>();
        registry->registerComponent<SpeedComponent>();
    }

    std::shared_ptr<ARegistry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<MovementSystem> system;
};

TEST_F(MovementSystemTest, NoEntities_NoUpdate) {
    system->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(MovementSystemTest, EntityWithIntentAndTransform_UsesBaseSpeed) {
    // Create entity with intent and transform, no speed component
    size_t entityId = 0;
    auto intent = std::make_shared<MovementIntentComponent>(math::Vector2f(1.0f, 0.0f), true);
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<MovementIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);

    float deltaTime = 0.016f;
    system->update(resourceManager, registry, deltaTime);

    // Position should be updated: 0 + (1,0) * 100 * 0.016 = (1.6, 0)
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 1.6f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);

    // Intent should be processed
    auto updatedIntent = registry->getComponent<MovementIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);
}

TEST_F(MovementSystemTest, EntityWithSpeedComponent_UsesCustomSpeed) {
    // Create entity with intent, transform, and speed
    size_t entityId = 0;
    auto intent = std::make_shared<MovementIntentComponent>(math::Vector2f(0.0f, 1.0f), true);
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(10.0f, 20.0f));
    auto speed = std::make_shared<SpeedComponent>(50.0f);

    registry->addComponent<MovementIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<SpeedComponent>(entityId, speed);

    float deltaTime = 0.02f;
    system->update(resourceManager, registry, deltaTime);

    // Position: (10,20) + (0,1) * 50 * 0.02 = (10, 20 + 1) = (10, 21)
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 10.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 21.0f);

    // Intent processed
    auto updatedIntent = registry->getComponent<MovementIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Processed);
}

TEST_F(MovementSystemTest, InactiveIntent_NoUpdate) {
    // Create entity with inactive intent
    size_t entityId = 0;
    auto intent = std::make_shared<MovementIntentComponent>(math::Vector2f(1.0f, 1.0f), false);
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<MovementIntentComponent>(entityId, intent);
    registry->addComponent<TransformComponent>(entityId, transform);

    system->update(resourceManager, registry, 0.016f);

    // Position should remain unchanged
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 0.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);

    // Intent should remain Temporary (not Processed)
    auto updatedIntent = registry->getComponent<MovementIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Temporary);
}

TEST_F(MovementSystemTest, EntityWithoutTransform_NoUpdate) {
    // Entity with intent but no transform
    size_t entityId = 0;
    auto intent = std::make_shared<MovementIntentComponent>(math::Vector2f(1.0f, 0.0f), true);

    registry->addComponent<MovementIntentComponent>(entityId, intent);

    system->update(resourceManager, registry, 0.016f);

    // No crash, intent not processed
    auto updatedIntent = registry->getComponent<MovementIntentComponent>(entityId);
    EXPECT_EQ(updatedIntent->getState(), ComponentState::Temporary);
}

TEST_F(MovementSystemTest, EntityWithoutIntent_NoUpdate) {
    // Entity with transform but no intent
    size_t entityId = 0;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));

    registry->addComponent<TransformComponent>(entityId, transform);

    system->update(resourceManager, registry, 0.016f);

    // Position unchanged
    auto updatedTransform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_EQ(updatedTransform->getPosition().getX(), 0.0f);
    EXPECT_EQ(updatedTransform->getPosition().getY(), 0.0f);
}

TEST_F(MovementSystemTest, MultipleEntities_UpdatesCorrectly) {
    // Entity 0: with speed
    size_t entityId0 = 0;
    auto intent0 = std::make_shared<MovementIntentComponent>(math::Vector2f(1.0f, 0.0f), true);
    auto transform0 = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto speed0 = std::make_shared<SpeedComponent>(200.0f);
    registry->addComponent<MovementIntentComponent>(entityId0, intent0);
    registry->addComponent<TransformComponent>(entityId0, transform0);
    registry->addComponent<SpeedComponent>(entityId0, speed0);

    // Entity 1: base speed
    size_t entityId1 = 1;
    auto intent1 = std::make_shared<MovementIntentComponent>(math::Vector2f(0.0f, -1.0f), true);
    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent<MovementIntentComponent>(entityId1, intent1);
    registry->addComponent<TransformComponent>(entityId1, transform1);

    float deltaTime = 0.01f;
    system->update(resourceManager, registry, deltaTime);

    // Entity 0: (0,0) + (1,0)*200*0.01 = (2, 0)
    auto updatedTransform0 = registry->getComponent<TransformComponent>(entityId0);
    EXPECT_EQ(updatedTransform0->getPosition().getX(), 2.0f);
    EXPECT_EQ(updatedTransform0->getPosition().getY(), 0.0f);

    // Entity 1: (100,100) + (0,-1)*100*0.01 = (100, 99)
    auto updatedTransform1 = registry->getComponent<TransformComponent>(entityId1);
    EXPECT_EQ(updatedTransform1->getPosition().getX(), 100.0f);
    EXPECT_EQ(updatedTransform1->getPosition().getY(), 99.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
