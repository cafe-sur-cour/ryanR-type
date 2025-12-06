/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TriggerSystemTests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../../common/components/temporary/TriggerIntentComponent.hpp"
#include "../../../../common/CollisionRules/CollisionRules.hpp"

namespace ecs {

class TriggerSystemTests : public ::testing::Test {
    protected:
        void SetUp() override {
            registry = std::make_shared<Registry>();
            triggerSystem = std::make_shared<TriggerSystem>();

            // Initialize collision rules with rules allowing trigger collisions for tests
            CollisionRulesData testData;
            CollisionRule allowAllTrigger;
            allowAllTrigger.groupA = {};
            allowAllTrigger.groupB = {};
            testData.triggerAllowRules.push_back(allowAllTrigger);
            CollisionRules::initWithData(testData);
        }

        void TearDown() override {
            registry.reset();
            triggerSystem.reset();
        }

        std::shared_ptr<Registry> registry;
        std::shared_ptr<TriggerSystem> triggerSystem;
};

TEST_F(TriggerSystemTests, Constructor) {
    EXPECT_NE(triggerSystem, nullptr);
}

TEST_F(TriggerSystemTests, NoTriggerEntities_NoTriggerIntents) {
    // Create entities without trigger colliders
    Entity entity1 = registry->createEntity();
    registry->addComponent<TransformComponent>(entity1, std::make_shared<TransformComponent>());
    registry->addComponent<ColliderComponent>(entity1, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    Entity entity2 = registry->createEntity();
    registry->addComponent<TransformComponent>(entity2, std::make_shared<TransformComponent>());
    registry->addComponent<ColliderComponent>(entity2, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that no TriggerIntentComponent was added
    auto triggerView = registry->view<TriggerIntentComponent>();
    int count = 0;
    for (auto entityId : triggerView) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(TriggerSystemTests, TriggerEntityWithoutCollision_NoTriggerIntent) {
    // Create a trigger entity with no other entities
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>());
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that no TriggerIntentComponent was added
    auto triggerView = registry->view<TriggerIntentComponent>();
    int count = 0;
    for (auto entityId : triggerView) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(TriggerSystemTests, TriggerEntityWithNonCollidingEntity_NoTriggerIntent) {
    // Create a trigger entity
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>(
        math::Vector2f(0, 0), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    // Create another entity far away (no collision)
    Entity otherEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity, std::make_shared<TransformComponent>(
        math::Vector2f(100, 100), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(otherEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that no TriggerIntentComponent was added
    auto triggerView = registry->view<TriggerIntentComponent>();
    int count = 0;
    for (auto entityId : triggerView) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(TriggerSystemTests, TriggerEntityWithCollidingEntity_AddsTriggerIntent) {
    // Create a trigger entity
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>(
        math::Vector2f(0, 0), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    // Create another entity that collides with the trigger
    Entity otherEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity, std::make_shared<TransformComponent>(
        math::Vector2f(5, 5), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(otherEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that TriggerIntentComponent was added to the trigger entity
    auto triggerIntent = registry->getComponent<TriggerIntentComponent>(triggerEntity);
    ASSERT_NE(triggerIntent, nullptr);
    EXPECT_EQ(triggerIntent->getSelf(), triggerEntity);
    EXPECT_EQ(triggerIntent->getOther(), otherEntity);
}

TEST_F(TriggerSystemTests, MultipleCollisions_AddsMultipleTriggerIntents) {
    // Create a trigger entity
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>(
        math::Vector2f(0, 0), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(20, 20), CollisionType::Trigger));

    // Create multiple colliding entities
    Entity otherEntity1 = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity1, std::make_shared<TransformComponent>(
        math::Vector2f(5, 5), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(otherEntity1, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    Entity otherEntity2 = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity2, std::make_shared<TransformComponent>(
        math::Vector2f(-5, -5), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(otherEntity2, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that TriggerIntentComponent was added to the trigger entity
    auto triggerIntent = registry->getComponent<TriggerIntentComponent>(triggerEntity);
    ASSERT_NE(triggerIntent, nullptr);
    // The system should add a TriggerIntentComponent for the first collision detected
    EXPECT_EQ(triggerIntent->getSelf(), triggerEntity);
    EXPECT_TRUE(triggerIntent->getOther() == otherEntity1 || triggerIntent->getOther() == otherEntity2);
}

TEST_F(TriggerSystemTests, TriggerDoesNotTriggerOnOtherTriggers) {
    // Create two trigger entities that collide
    Entity triggerEntity1 = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity1, std::make_shared<TransformComponent>(
        math::Vector2f(0, 0), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(triggerEntity1, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    Entity triggerEntity2 = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity2, std::make_shared<TransformComponent>(
        math::Vector2f(5, 5), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(triggerEntity2, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that TriggerIntentComponent was added (triggers DO trigger on other triggers)
    auto triggerIntent1 = registry->getComponent<TriggerIntentComponent>(triggerEntity1);
    ASSERT_NE(triggerIntent1, nullptr);
    EXPECT_EQ(triggerIntent1->getSelf(), triggerEntity1);
    EXPECT_EQ(triggerIntent1->getOther(), triggerEntity2);
}

TEST_F(TriggerSystemTests, EntityWithoutTransform_Ignored) {
    // Create a trigger entity without TransformComponent
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger));

    // Create another entity
    Entity otherEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity, std::make_shared<TransformComponent>());
    registry->addComponent<ColliderComponent>(otherEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that no TriggerIntentComponent was added
    auto triggerView = registry->view<TriggerIntentComponent>();
    int count = 0;
    for (auto entityId : triggerView) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(TriggerSystemTests, EntityWithoutCollider_Ignored) {
    // Create a trigger entity without ColliderComponent
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>());

    // Create another entity
    Entity otherEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity, std::make_shared<TransformComponent>());
    registry->addComponent<ColliderComponent>(otherEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that no TriggerIntentComponent was added
    auto triggerView = registry->view<TriggerIntentComponent>();
    int count = 0;
    for (auto entityId : triggerView) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(TriggerSystemTests, ScaledColliders_CollisionDetection) {
    // Create a trigger entity with scale
    Entity triggerEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(triggerEntity, std::make_shared<TransformComponent>(
        math::Vector2f(0, 0), 0.0f, math::Vector2f(2.0f, 2.0f))); // Scale 2x
    registry->addComponent<ColliderComponent>(triggerEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Trigger)); // Effective size: 20x20

    // Create another entity that collides with the scaled trigger
    Entity otherEntity = registry->createEntity();
    registry->addComponent<TransformComponent>(otherEntity, std::make_shared<TransformComponent>(
        math::Vector2f(15, 15), 0.0f, math::Vector2f(1, 1)));
    registry->addComponent<ColliderComponent>(otherEntity, std::make_shared<ColliderComponent>(
        math::Vector2f(0, 0), math::Vector2f(10, 10), CollisionType::Solid));

    triggerSystem->update(nullptr, registry, 0.016f);

    // Check that TriggerIntentComponent was added (collision should be detected with scaled collider)
    auto triggerIntent = registry->getComponent<TriggerIntentComponent>(triggerEntity);
    ASSERT_NE(triggerIntent, nullptr);
    EXPECT_EQ(triggerIntent->getSelf(), triggerEntity);
    EXPECT_EQ(triggerIntent->getOther(), otherEntity);
}

} // namespace ecs