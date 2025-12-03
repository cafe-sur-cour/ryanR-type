/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/DeathIntentComponent.hpp"

using namespace ecs;

/* DeathSystem Tests */

class DeathSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        deathSystem = std::make_shared<DeathSystem>();

        // Register components
        registry->registerComponent<DeathIntentComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<DeathSystem> deathSystem;
};

TEST_F(DeathSystemTest, NoEntities_NoUpdate) {
    deathSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(DeathSystemTest, EntityWithDeathIntent_Destroyed) {
    // Create entity with death intent
    ecs::Entity entityId = 0;
    auto deathIntent = std::make_shared<DeathIntentComponent>();

    registry->addComponent<DeathIntentComponent>(entityId, deathIntent);

    // Update
    deathSystem->update(resourceManager, registry, 0.016f);

    // Entity destroyed
    EXPECT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(DeathSystemTest, EntityWithoutDeathIntent_NotDestroyed) {
    // Create entity without death intent
    ecs::Entity entityId = 0;

    // Update
    deathSystem->update(resourceManager, registry, 0.016f);

    // Entity still exists (though no components)
    // Since no component was added, hasComponent returns false anyway
    EXPECT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(DeathSystemTest, MultipleEntities_SomeDestroyed) {
    // Entity 0: has death intent, should be destroyed
    ecs::Entity entityId0 = 0;
    auto deathIntent0 = std::make_shared<DeathIntentComponent>();
    registry->addComponent<DeathIntentComponent>(entityId0, deathIntent0);

    // Entity 1: no death intent, should survive
    ecs::Entity entityId1 = 1;

    // Entity 2: has death intent, should be destroyed
    ecs::Entity entityId2 = 2;
    auto deathIntent2 = std::make_shared<DeathIntentComponent>();
    registry->addComponent<DeathIntentComponent>(entityId2, deathIntent2);

    // Update
    deathSystem->update(resourceManager, registry, 0.016f);

    // Entity 0: destroyed
    EXPECT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId0));

    // Entity 1: still exists
    EXPECT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId1));

    // Entity 2: destroyed
    EXPECT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}