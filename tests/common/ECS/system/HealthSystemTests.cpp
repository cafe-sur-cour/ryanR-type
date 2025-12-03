/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/HealthComponent.hpp"
#include "../../../../common/components/temporary/DamageIntentComponent.hpp"

using namespace ecs;

/* HealthSystem Tests */

class HealthSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        healthSystem = std::make_shared<HealthSystem>();

        // Register components
        registry->registerComponent<HealthComponent>();
        registry->registerComponent<DamageIntentComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<HealthSystem> healthSystem;
};

TEST_F(HealthSystemTest, NoEntities_NoUpdate) {
    healthSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(HealthSystemTest, EntityWithHealth_NoDamage_NoChange) {
    // Create entity with health only
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(100.0f);

    registry->addComponent<HealthComponent>(entityId, health);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Health unchanged
    auto updatedHealth = registry->getComponent<HealthComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedHealth->getHealth(), 100.0f);

    // Entity not destroyed
    EXPECT_TRUE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, EntityWithHealthAndDamage_AppliesDamage) {
    // Create entity with health and damage intent
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(100.0f);
    auto damage = std::make_shared<DamageIntentComponent>(25.0f);

    registry->addComponent<HealthComponent>(entityId, health);
    registry->addComponent<DamageIntentComponent>(entityId, damage);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Health reduced
    auto updatedHealth = registry->getComponent<HealthComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedHealth->getHealth(), 75.0f);

    // Entity not destroyed
    EXPECT_TRUE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, EntityWithZeroHealthAfterDamage_Destroyed) {
    // Create entity with low health
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(25.0f);
    auto damage = std::make_shared<DamageIntentComponent>(25.0f);

    registry->addComponent<HealthComponent>(entityId, health);
    registry->addComponent<DamageIntentComponent>(entityId, damage);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Health zero
    // But since damage is applied first, health becomes 0, then in _handleHealthUpdates, <=0 destroys

    // Entity destroyed
    EXPECT_FALSE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, EntityWithNegativeHealthAfterDamage_Destroyed) {
    // Create entity with health
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(20.0f);
    auto damage = std::make_shared<DamageIntentComponent>(30.0f);

    registry->addComponent<HealthComponent>(entityId, health);
    registry->addComponent<DamageIntentComponent>(entityId, damage);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Health negative, entity destroyed
    EXPECT_FALSE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, EntityWithHealthZero_Destroyed) {
    // Create entity with zero health
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(0.0f);

    registry->addComponent<HealthComponent>(entityId, health);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Entity destroyed
    EXPECT_FALSE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, EntityWithNegativeHealth_Destroyed) {
    // Create entity with negative health
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(-10.0f);

    registry->addComponent<HealthComponent>(entityId, health);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Entity destroyed
    EXPECT_FALSE(registry->hasComponent<HealthComponent>(entityId));
}

TEST_F(HealthSystemTest, MultipleEntities_UpdatesCorrectly) {
    // Entity 0: health 100, damage 20 -> health 80, survives
    ecs::Entity entityId0 = 0;
    auto health0 = std::make_shared<HealthComponent>(100.0f);
    auto damage0 = std::make_shared<DamageIntentComponent>(20.0f);
    registry->addComponent<HealthComponent>(entityId0, health0);
    registry->addComponent<DamageIntentComponent>(entityId0, damage0);

    // Entity 1: health 50, damage 50 -> health 0, destroyed
    ecs::Entity entityId1 = 1;
    auto health1 = std::make_shared<HealthComponent>(50.0f);
    auto damage1 = std::make_shared<DamageIntentComponent>(50.0f);
    registry->addComponent<HealthComponent>(entityId1, health1);
    registry->addComponent<DamageIntentComponent>(entityId1, damage1);

    // Entity 2: health 30, no damage -> health 30, survives
    ecs::Entity entityId2 = 2;
    auto health2 = std::make_shared<HealthComponent>(30.0f);
    registry->addComponent<HealthComponent>(entityId2, health2);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Entity 0: health 80, exists
    EXPECT_TRUE(registry->hasComponent<HealthComponent>(entityId0));
    auto updatedHealth0 = registry->getComponent<HealthComponent>(entityId0);
    EXPECT_FLOAT_EQ(updatedHealth0->getHealth(), 80.0f);

    // Entity 1: destroyed
    EXPECT_FALSE(registry->hasComponent<HealthComponent>(entityId1));

    // Entity 2: health 30, exists
    EXPECT_TRUE(registry->hasComponent<HealthComponent>(entityId2));
    auto updatedHealth2 = registry->getComponent<HealthComponent>(entityId2);
    EXPECT_FLOAT_EQ(updatedHealth2->getHealth(), 30.0f);
}

TEST_F(HealthSystemTest, DamageIntentRemovedAfterUpdate) {
    // Create entity
    ecs::Entity entityId = 0;
    auto health = std::make_shared<HealthComponent>(100.0f);
    auto damage = std::make_shared<DamageIntentComponent>(10.0f);

    registry->addComponent<HealthComponent>(entityId, health);
    registry->addComponent<DamageIntentComponent>(entityId, damage);

    // Update
    healthSystem->update(resourceManager, registry, 0.016f);

    // Damage intent deleted
    EXPECT_FALSE(registry->hasComponent<DamageIntentComponent>(entityId));
    EXPECT_EQ(health->getHealth(), 90.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}