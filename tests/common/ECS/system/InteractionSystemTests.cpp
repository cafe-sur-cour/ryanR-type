/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/TriggerIntentComponent.hpp"
#include "../../../../common/components/permanent/InteractionConfigComponent.hpp"
#include "../../../../common/components/permanent/DamageCooldownComponent.hpp"

using namespace ecs;

/* InteractionSystem Tests */

class InteractionSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        interactionSystem = std::make_shared<InteractionSystem>();

        // Register components
        registry->registerComponent<TriggerIntentComponent>();
        registry->registerComponent<InteractionConfigComponent>();
        registry->registerComponent<DamageCooldownComponent>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<InteractionSystem> interactionSystem;
};

TEST_F(InteractionSystemTest, NoEntities_NoUpdate) {
    interactionSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(InteractionSystemTest, EntityWithoutInteractionConfig_TriggerIntentRemoved) {
    // Create entity with trigger intent but no interaction config
    ecs::Entity entityId = 0;
    ecs::Entity otherId = 1;
    auto triggerIntent = std::make_shared<TriggerIntentComponent>(otherId);

    registry->addComponent<TriggerIntentComponent>(entityId, triggerIntent);

    // Update
    interactionSystem->update(resourceManager, registry, 0.016f);

    // Trigger intent should be removed
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entityId));
}

TEST_F(InteractionSystemTest, DamageCooldownComponent_UpdatesOverTime) {
    // Create entity with damage cooldown component
    ecs::Entity entityId = 0;
    auto cooldownComp = std::make_shared<DamageCooldownComponent>(1.0f);

    registry->addComponent<DamageCooldownComponent>(entityId, cooldownComp);

    // Update with deltaTime
    interactionSystem->update(resourceManager, registry, 0.5f);

    // Cooldown time should increase
    auto updatedCooldown = registry->getComponent<DamageCooldownComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedCooldown->getLastDamageTime(), 0.5f);
}

TEST_F(InteractionSystemTest, MultipleDamageCooldowns_AllUpdated) {
    // Create multiple entities with damage cooldown
    ecs::Entity entity1Id = 0;
    auto cooldown1 = std::make_shared<DamageCooldownComponent>(1.0f);
    registry->addComponent<DamageCooldownComponent>(entity1Id, cooldown1);

    ecs::Entity entity2Id = 1;
    auto cooldown2 = std::make_shared<DamageCooldownComponent>(2.0f);
    registry->addComponent<DamageCooldownComponent>(entity2Id, cooldown2);
    // Set initial lastDamageTime for entity2
    cooldown2->setLastDamageTime(0.5f);

    // Update
    interactionSystem->update(resourceManager, registry, 0.1f);

    // Both should be updated
    auto updated1 = registry->getComponent<DamageCooldownComponent>(entity1Id);
    EXPECT_FLOAT_EQ(updated1->getLastDamageTime(), 0.1f);

    auto updated2 = registry->getComponent<DamageCooldownComponent>(entity2Id);
    EXPECT_FLOAT_EQ(updated2->getLastDamageTime(), 0.6f);
}

TEST_F(InteractionSystemTest, TriggerIntentAlwaysRemoved) {
    // Create entity with trigger intent and interaction config
    ecs::Entity entityId = 0;
    ecs::Entity otherId = 1;
    auto triggerIntent = std::make_shared<TriggerIntentComponent>(otherId);
    auto interactionConfig = std::make_shared<InteractionConfigComponent>();

    registry->addComponent<TriggerIntentComponent>(entityId, triggerIntent);
    registry->addComponent<InteractionConfigComponent>(entityId, interactionConfig);

    // Update
    interactionSystem->update(resourceManager, registry, 0.016f);

    // Trigger intent should be removed even with empty config
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entityId));
}

TEST_F(InteractionSystemTest, MultipleTriggerIntents_AllRemoved) {
    // Create multiple entities with trigger intents
    ecs::Entity entity1Id = 0;
    ecs::Entity other1Id = 2;
    auto triggerIntent1 = std::make_shared<TriggerIntentComponent>(other1Id);
    auto interactionConfig1 = std::make_shared<InteractionConfigComponent>();
    registry->addComponent<TriggerIntentComponent>(entity1Id, triggerIntent1);
    registry->addComponent<InteractionConfigComponent>(entity1Id, interactionConfig1);

    ecs::Entity entity2Id = 1;
    ecs::Entity other2Id = 3;
    auto triggerIntent2 = std::make_shared<TriggerIntentComponent>(other2Id);
    registry->addComponent<TriggerIntentComponent>(entity2Id, triggerIntent2);

    // Update
    interactionSystem->update(resourceManager, registry, 0.016f);

    // All trigger intents should be removed
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entity1Id));
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entity2Id));
}

TEST_F(InteractionSystemTest, EmptyInteractionConfig_NoAction) {
    // Create entity with empty interaction config
    ecs::Entity entityId = 0;
    ecs::Entity otherId = 1;
    auto triggerIntent = std::make_shared<TriggerIntentComponent>(otherId);
    auto interactionConfig = std::make_shared<InteractionConfigComponent>();

    registry->addComponent<TriggerIntentComponent>(entityId, triggerIntent);
    registry->addComponent<InteractionConfigComponent>(entityId, interactionConfig);

    // Update
    interactionSystem->update(resourceManager, registry, 0.016f);

    // Should not crash, trigger intent removed
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entityId));
}

TEST_F(InteractionSystemTest, DamageCooldownAccumulates) {
    // Create entity with damage cooldown
    ecs::Entity entityId = 0;
    auto cooldownComp = std::make_shared<DamageCooldownComponent>(1.0f);

    registry->addComponent<DamageCooldownComponent>(entityId, cooldownComp);

    // Update multiple times
    interactionSystem->update(resourceManager, registry, 0.1f);
    interactionSystem->update(resourceManager, registry, 0.2f);
    interactionSystem->update(resourceManager, registry, 0.3f);

    // Cooldown should accumulate
    auto updatedCooldown = registry->getComponent<DamageCooldownComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedCooldown->getLastDamageTime(), 0.6f);
}

TEST_F(InteractionSystemTest, ZeroDeltaTime_NoCooldownIncrease) {
    // Create entity with damage cooldown
    ecs::Entity entityId = 0;
    auto cooldownComp = std::make_shared<DamageCooldownComponent>(1.0f);

    registry->addComponent<DamageCooldownComponent>(entityId, cooldownComp);

    // Update with zero deltaTime
    interactionSystem->update(resourceManager, registry, 0.0f);

    // Cooldown should not change (should remain at 0 since it's initialized at 0)
    auto updatedCooldown = registry->getComponent<DamageCooldownComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedCooldown->getLastDamageTime(), 0.0f);
}

TEST_F(InteractionSystemTest, InteractionConfigWithMapping_NoTagMatch_NoAction) {
    // Create entity with interaction config that has mappings
    ecs::Entity entityId = 0;
    ecs::Entity otherId = 1;
    auto triggerIntent = std::make_shared<TriggerIntentComponent>(otherId);

    InteractionMapping mapping;
    mapping.targetTags = {"SomeTag"};
    mapping.actionsToOther = {"SomeAction"};
    mapping.actionsToSelf = {};

    auto interactionConfig = std::make_shared<InteractionConfigComponent>();
    interactionConfig->addMapping(mapping);

    registry->addComponent<TriggerIntentComponent>(entityId, triggerIntent);
    registry->addComponent<InteractionConfigComponent>(entityId, interactionConfig);

    // Update (other entity doesn't have the tag, so no action should occur)
    interactionSystem->update(resourceManager, registry, 0.016f);

    // Trigger intent should still be removed
    EXPECT_FALSE(registry->hasComponent<TriggerIntentComponent>(entityId));
}

TEST_F(InteractionSystemTest, LargeDeltaTime_CooldownUpdatesCorrectly) {
    // Create entity with damage cooldown
    ecs::Entity entityId = 0;
    auto cooldownComp = std::make_shared<DamageCooldownComponent>(1.0f);

    registry->addComponent<DamageCooldownComponent>(entityId, cooldownComp);

    // Update with large deltaTime
    interactionSystem->update(resourceManager, registry, 10.0f);

    // Cooldown should increase by large amount
    auto updatedCooldown = registry->getComponent<DamageCooldownComponent>(entityId);
    EXPECT_FLOAT_EQ(updatedCooldown->getLastDamageTime(), 10.0f);
}
