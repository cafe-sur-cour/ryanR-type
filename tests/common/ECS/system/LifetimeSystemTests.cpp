/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/LifetimeComponent.hpp"

using namespace ecs;

/* LifetimeSystem Tests */

class LifetimeSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        lifetimeSystem = std::make_shared<LifetimeSystem>();

        // Register components
        registry->registerComponent<LifetimeComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<LifetimeSystem> lifetimeSystem;
};

TEST_F(LifetimeSystemTest, NoEntities_NoUpdate) {
    lifetimeSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(LifetimeSystemTest, EntityWithLifetimeComponent_DecreasesLifetime) {
    // Create entity with lifetime component
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(5.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with deltaTime
    float deltaTime = 1.0f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Lifetime should be decreased
    auto updatedLifetime = registry->getComponent<LifetimeComponent>(entityId);
    ASSERT_TRUE(updatedLifetime);
    EXPECT_FLOAT_EQ(updatedLifetime->getLifetime(), 4.0f);
}

TEST_F(LifetimeSystemTest, EntityWithLifetimeComponent_MultipleUpdates) {
    // Create entity with lifetime component
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(10.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // First update
    lifetimeSystem->update(resourceManager, registry, 2.5f);
    auto updatedLifetime1 = registry->getComponent<LifetimeComponent>(entityId);
    ASSERT_TRUE(updatedLifetime1);
    EXPECT_FLOAT_EQ(updatedLifetime1->getLifetime(), 7.5f);

    // Second update
    lifetimeSystem->update(resourceManager, registry, 1.0f);
    auto updatedLifetime2 = registry->getComponent<LifetimeComponent>(entityId);
    ASSERT_TRUE(updatedLifetime2);
    EXPECT_FLOAT_EQ(updatedLifetime2->getLifetime(), 6.5f);
}

TEST_F(LifetimeSystemTest, EntityLifetimeReachesZero_DestroysEntity) {
    // Create entity with lifetime component
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(2.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with deltaTime that will make lifetime <= 0
    float deltaTime = 2.5f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

TEST_F(LifetimeSystemTest, EntityLifetimeBecomesNegative_DestroysEntity) {
    // Create entity with lifetime component
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(1.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with deltaTime that will make lifetime negative
    float deltaTime = 2.0f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

TEST_F(LifetimeSystemTest, EntityLifetimeExactlyZero_DestroysEntity) {
    // Create entity with lifetime component
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(1.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with exact deltaTime to reach zero
    float deltaTime = 1.0f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

TEST_F(LifetimeSystemTest, EntityWithZeroLifetime_DestroysImmediately) {
    // Create entity with zero lifetime
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(0.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with any deltaTime
    float deltaTime = 0.1f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

TEST_F(LifetimeSystemTest, EntityWithNegativeLifetime_DestroysImmediately) {
    // Create entity with negative lifetime
    ecs::Entity entityId = registry->createEntity();
    auto lifetimeComp = std::make_shared<LifetimeComponent>(-1.0f);
    registry->addComponent<LifetimeComponent>(entityId, lifetimeComp);

    // Update with any deltaTime
    float deltaTime = 0.1f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // Entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

TEST_F(LifetimeSystemTest, MultipleEntities_DifferentLifetimes) {
    // Create first entity with long lifetime
    ecs::Entity entityId1 = registry->createEntity();
    auto lifetimeComp1 = std::make_shared<LifetimeComponent>(10.0f);
    registry->addComponent<LifetimeComponent>(entityId1, lifetimeComp1);

    // Create second entity with short lifetime
    ecs::Entity entityId2 = registry->createEntity();
    auto lifetimeComp2 = std::make_shared<LifetimeComponent>(1.0f);
    registry->addComponent<LifetimeComponent>(entityId2, lifetimeComp2);

    // Update
    float deltaTime = 1.5f;
    lifetimeSystem->update(resourceManager, registry, deltaTime);

    // First entity should still exist with decreased lifetime
    EXPECT_TRUE(registry->hasComponent<LifetimeComponent>(entityId1));
    auto updatedLifetime1 = registry->getComponent<LifetimeComponent>(entityId1);
    EXPECT_FLOAT_EQ(updatedLifetime1->getLifetime(), 8.5f);

    // Second entity should be destroyed
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId2));
}

TEST_F(LifetimeSystemTest, EntityWithoutLifetimeComponent_Ignored) {
    // Create entity without lifetime component
    ecs::Entity entityId = registry->createEntity();

    // Update
    lifetimeSystem->update(resourceManager, registry, 1.0f);

    // Entity should still exist (no component to check)
    // Since no component was added, hasComponent should return false anyway
    EXPECT_FALSE(registry->hasComponent<LifetimeComponent>(entityId));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}