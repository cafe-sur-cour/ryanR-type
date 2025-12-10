/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** OutOfBoundsSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../common/components/permanent/GameZoneComponent.hpp"
#include "../../../../common/components/temporary/DeathIntentComponent.hpp"
#include "../../../../common/components/tags/PlayerProjectileTag.hpp"
#include "../../../../common/components/tags/EnnemyProjectileTag.hpp"
#include "../../../../common/components/tags/MobTag.hpp"
#include "../../../../common/constants.hpp"

using namespace ecs;

/* OutOfBoundsSystem Tests */

class OutOfBoundsSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        outOfBoundsSystem = std::make_shared<OutOfBoundsSystem>();

        // Register components
        registry->registerComponent<TransformComponent>();
        registry->registerComponent<GameZoneComponent>();
        registry->registerComponent<DeathIntentComponent>();
        registry->registerComponent<PlayerProjectileTag>();
        registry->registerComponent<EnnemyProjectileTag>();
        registry->registerComponent<MobTag>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<OutOfBoundsSystem> outOfBoundsSystem;
};

TEST_F(OutOfBoundsSystemTest, NoEntities_NoUpdate) {
    outOfBoundsSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(OutOfBoundsSystemTest, NoGameZone_UsesDefaultBounds) {
    // Create entity with PlayerProjectileTag inside default bounds
    ecs::Entity entityId = 0;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should not have DeathIntent
    ASSERT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, PlayerProjectileInsideBounds_NoDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag inside bounds
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should not have DeathIntent
    ASSERT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, PlayerProjectileOutsideLeftBound_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag outside left bound (margin = 500)
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, PlayerProjectileOutsideRightBound_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag outside right bound (1920 + 500 = 2420)
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(2500.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, PlayerProjectileOutsideTopBound_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag outside top bound (margin = 500)
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, -600.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, PlayerProjectileOutsideBottomBound_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag outside bottom bound (1080 + 500 = 1580)
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 1600.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, EnnemyProjectileOutsideBounds_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with EnnemyProjectileTag outside bounds
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<EnnemyProjectileTag>(entityId, std::make_shared<EnnemyProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, MobOutsideBounds_AddsDeathIntent) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with MobTag outside bounds
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<MobTag>(entityId, std::make_shared<MobTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, EntityWithoutRequiredTags_Ignored) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with TransformComponent but no tags
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should not have DeathIntent
    ASSERT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, EntityWithDeathIntentAlready_Ignored) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag and already has DeathIntent
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());
    registry->addComponent<DeathIntentComponent>(entityId, std::make_shared<DeathIntentComponent>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should still have only one DeathIntent (not add another)
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, GameZoneWithOffset_AdjustsBounds) {
    // Create game zone entity with offset
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity with PlayerProjectileTag outside adjusted left bound (100 - 500 = -400)
    ecs::Entity entityId = 1;
    auto transform = std::make_shared<TransformComponent>(math::Vector2f(-500.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId, transform);
    registry->addComponent<PlayerProjectileTag>(entityId, std::make_shared<PlayerProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // Should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId));
}

TEST_F(OutOfBoundsSystemTest, MultipleEntities_ProcessesAll) {
    // Create game zone entity
    ecs::Entity gameZoneId = 0;
    auto gameZoneTransform = std::make_shared<TransformComponent>(math::Vector2f(0.0f, 0.0f));
    auto gameZone = std::make_shared<GameZoneComponent>(math::FRect(0.0f, 0.0f, 1920.0f, 1080.0f));
    registry->addComponent<TransformComponent>(gameZoneId, gameZoneTransform);
    registry->addComponent<GameZoneComponent>(gameZoneId, gameZone);

    // Create entity inside bounds
    ecs::Entity entityId1 = 1;
    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(100.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId1, transform1);
    registry->addComponent<PlayerProjectileTag>(entityId1, std::make_shared<PlayerProjectileTag>());

    // Create entity outside bounds
    ecs::Entity entityId2 = 2;
    auto transform2 = std::make_shared<TransformComponent>(math::Vector2f(-600.0f, 100.0f));
    registry->addComponent<TransformComponent>(entityId2, transform2);
    registry->addComponent<EnnemyProjectileTag>(entityId2, std::make_shared<EnnemyProjectileTag>());

    outOfBoundsSystem->update(resourceManager, registry, 0.016f);

    // First entity should not have DeathIntent
    ASSERT_FALSE(registry->hasComponent<DeathIntentComponent>(entityId1));

    // Second entity should have DeathIntent
    ASSERT_TRUE(registry->hasComponent<DeathIntentComponent>(entityId2));
}