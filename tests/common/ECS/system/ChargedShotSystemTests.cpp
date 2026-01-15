/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/shooting/ChargedShotSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/ChargedShotComponent.hpp"

using namespace ecs;

/* ChargedShotSystem Tests */

class ChargedShotSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        chargedShotSystem = std::make_shared<ChargedShotSystem>();

        // Register components
        registry->registerComponent<ChargedShotComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<ChargedShotSystem> chargedShotSystem;
};

TEST_F(ChargedShotSystemTest, NoEntities_NoUpdate) {
    chargedShotSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(ChargedShotSystemTest, EntityWithChargedShot_IncreasesCharge) {
    // Create entity with charged shot component
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(0.0f, 1.0f, 2.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update with deltaTime
    chargedShotSystem->update(resourceManager, registry, 0.1f);

    // Check charge increased
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 0.1f);
}

TEST_F(ChargedShotSystemTest, ChargeAccumulates_OverMultipleUpdates) {
    // Create entity with charged shot component
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(0.0f, 2.0f, 3.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update multiple times
    chargedShotSystem->update(resourceManager, registry, 0.1f);
    chargedShotSystem->update(resourceManager, registry, 0.2f);
    chargedShotSystem->update(resourceManager, registry, 0.15f);

    // Check charge accumulated
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 0.45f);
}

TEST_F(ChargedShotSystemTest, ChargeCapped_AtMaxCharge) {
    // Create entity with charged shot component at 0.9 charge
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(0.9f, 1.0f, 2.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update with large deltaTime that would exceed max
    chargedShotSystem->update(resourceManager, registry, 0.5f);

    // Check charge capped at max
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 1.0f);
}

TEST_F(ChargedShotSystemTest, ChargeAtMax_StaysAtMax) {
    // Create entity with charged shot component at max charge
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(2.5f, 2.5f, 3.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update
    chargedShotSystem->update(resourceManager, registry, 0.1f);

    // Check charge stays at max
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 2.5f);
}

TEST_F(ChargedShotSystemTest, MultipleEntities_AllUpdated) {
    // Create multiple entities with charged shot components
    ecs::Entity entity1Id = 0;
    auto chargedShot1 = std::make_shared<ChargedShotComponent>(0.0f, 1.0f, 2.0f);
    registry->addComponent<ChargedShotComponent>(entity1Id, chargedShot1);

    ecs::Entity entity2Id = 1;
    auto chargedShot2 = std::make_shared<ChargedShotComponent>(0.5f, 2.0f, 3.0f);
    registry->addComponent<ChargedShotComponent>(entity2Id, chargedShot2);

    ecs::Entity entity3Id = 2;
    auto chargedShot3 = std::make_shared<ChargedShotComponent>(0.8f, 1.0f, 1.5f);
    registry->addComponent<ChargedShotComponent>(entity3Id, chargedShot3);

    // Update
    chargedShotSystem->update(resourceManager, registry, 0.1f);

    // Check all entities updated
    auto updated1 = registry->getComponent<ChargedShotComponent>(entity1Id);
    EXPECT_FLOAT_EQ(updated1->getCharge(), 0.1f);

    auto updated2 = registry->getComponent<ChargedShotComponent>(entity2Id);
    EXPECT_FLOAT_EQ(updated2->getCharge(), 0.6f);

    auto updated3 = registry->getComponent<ChargedShotComponent>(entity3Id);
    EXPECT_FLOAT_EQ(updated3->getCharge(), 0.9f);
}

TEST_F(ChargedShotSystemTest, ZeroDeltaTime_NoChange) {
    // Create entity with charged shot component
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(0.5f, 1.0f, 2.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update with zero deltaTime
    chargedShotSystem->update(resourceManager, registry, 0.0f);

    // Check charge unchanged
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 0.5f);
}

TEST_F(ChargedShotSystemTest, LargeDeltaTime_CappedAtMax) {
    // Create entity with low charge
    ecs::Entity entityId = 0;
    auto chargedShot = std::make_shared<ChargedShotComponent>(0.1f, 1.0f, 2.0f);

    registry->addComponent<ChargedShotComponent>(entityId, chargedShot);

    // Update with very large deltaTime
    chargedShotSystem->update(resourceManager, registry, 10.0f);

    // Check charge capped at max
    auto updatedChargedShot = registry->getComponent<ChargedShotComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedChargedShot->getCharge(), 1.0f);
}

TEST_F(ChargedShotSystemTest, DifferentMaxCharges_RespectedIndependently) {
    // Create entities with different max charges
    ecs::Entity entity1Id = 0;
    auto chargedShot1 = std::make_shared<ChargedShotComponent>(0.0f, 0.5f, 1.0f);
    registry->addComponent<ChargedShotComponent>(entity1Id, chargedShot1);

    ecs::Entity entity2Id = 1;
    auto chargedShot2 = std::make_shared<ChargedShotComponent>(0.0f, 3.0f, 2.0f);
    registry->addComponent<ChargedShotComponent>(entity2Id, chargedShot2);

    // Update with large deltaTime
    chargedShotSystem->update(resourceManager, registry, 5.0f);

    // Check each capped at their own max
    auto updated1 = registry->getComponent<ChargedShotComponent>(entity1Id);
    EXPECT_FLOAT_EQ(updated1->getCharge(), 0.5f);

    auto updated2 = registry->getComponent<ChargedShotComponent>(entity2Id);
    EXPECT_FLOAT_EQ(updated2->getCharge(), 3.0f);
}
