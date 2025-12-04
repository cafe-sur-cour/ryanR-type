/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/permanent/ScoreComponent.hpp"
#include "../../../../common/components/temporary/ScoreIntentComponent.hpp"
#include "../../../../common/components/tags/ScoreTag.hpp"

using namespace ecs;

/* ScoreSystem Tests */

class ScoreSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        scoreSystem = std::make_shared<ScoreSystem>();

        // Register components
        registry->registerComponent<ScoreComponent>();
        registry->registerComponent<ScoreIntentComponent>();
        registry->registerComponent<ScoreTag>();
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<ScoreSystem> scoreSystem;
};

TEST_F(ScoreSystemTest, NoEntities_NoUpdate) {
    scoreSystem->update(resourceManager, registry, 0.016f);
    // No crash, no changes
}

TEST_F(ScoreSystemTest, EntityWithScoreTagAndScore_NoIntent_NoChange) {
    // Create entity with ScoreTag and ScoreComponent, no intent
    ecs::Entity entityId = 0;
    auto score = std::make_shared<ScoreComponent>(50);
    auto tag = std::make_shared<ScoreTag>();

    registry->addComponent<ScoreTag>(entityId, tag);
    registry->addComponent<ScoreComponent>(entityId, score);

    // Update
    scoreSystem->update(resourceManager, registry, 0.016f);

    // Check score unchanged
    auto updatedScore = registry->getComponent<ScoreComponent>(entityId);
    ASSERT_TRUE(updatedScore);
    EXPECT_EQ(updatedScore->getScore(), 50);

    // No intent should be present
    auto intent = registry->getComponent<ScoreIntentComponent>(entityId);
    EXPECT_FALSE(intent);
}

TEST_F(ScoreSystemTest, EntityWithScoreTagScoreAndIntent_ScoreUpdatedIntentRemoved) {
    // Create entity with ScoreTag, ScoreComponent, and ScoreIntent
    ecs::Entity entityId = 0;
    auto score = std::make_shared<ScoreComponent>(50);
    auto tag = std::make_shared<ScoreTag>();
    auto intent = std::make_shared<ScoreIntentComponent>(25);

    registry->addComponent<ScoreTag>(entityId, tag);
    registry->addComponent<ScoreComponent>(entityId, score);
    registry->addComponent<ScoreIntentComponent>(entityId, intent);

    // Update
    scoreSystem->update(resourceManager, registry, 0.016f);

    // Check score updated
    auto updatedScore = registry->getComponent<ScoreComponent>(entityId);
    ASSERT_TRUE(updatedScore);
    EXPECT_EQ(updatedScore->getScore(), 75);  // 50 + 25

    // Intent should be removed
    auto removedIntent = registry->getComponent<ScoreIntentComponent>(entityId);
    EXPECT_FALSE(removedIntent);
}

TEST_F(ScoreSystemTest, EntityWithoutScoreTag_NoProcessing) {
    // Create entity with ScoreComponent and ScoreIntent, but no ScoreTag
    ecs::Entity entityId = 0;
    auto score = std::make_shared<ScoreComponent>(50);
    auto intent = std::make_shared<ScoreIntentComponent>(25);

    registry->addComponent<ScoreComponent>(entityId, score);
    registry->addComponent<ScoreIntentComponent>(entityId, intent);

    // Update
    scoreSystem->update(resourceManager, registry, 0.016f);

    // Check score unchanged
    auto updatedScore = registry->getComponent<ScoreComponent>(entityId);
    ASSERT_TRUE(updatedScore);
    EXPECT_EQ(updatedScore->getScore(), 50);

    // Intent should still be present
    auto stillIntent = registry->getComponent<ScoreIntentComponent>(entityId);
    ASSERT_TRUE(stillIntent);
    EXPECT_EQ(stillIntent->getScore(), 25);
}

TEST_F(ScoreSystemTest, MultipleEntities_OnlyTaggedProcessed) {
    // Entity 0: with tag, score, intent
    ecs::Entity entity0 = 0;
    auto score0 = std::make_shared<ScoreComponent>(10);
    auto tag0 = std::make_shared<ScoreTag>();
    auto intent0 = std::make_shared<ScoreIntentComponent>(5);
    registry->addComponent<ScoreTag>(entity0, tag0);
    registry->addComponent<ScoreComponent>(entity0, score0);
    registry->addComponent<ScoreIntentComponent>(entity0, intent0);

    // Entity 1: without tag, with score and intent
    ecs::Entity entity1 = 1;
    auto score1 = std::make_shared<ScoreComponent>(20);
    auto intent1 = std::make_shared<ScoreIntentComponent>(10);
    registry->addComponent<ScoreComponent>(entity1, score1);
    registry->addComponent<ScoreIntentComponent>(entity1, intent1);

    // Update
    scoreSystem->update(resourceManager, registry, 0.016f);

    // Entity 0: score updated, intent removed
    auto updatedScore0 = registry->getComponent<ScoreComponent>(entity0);
    ASSERT_TRUE(updatedScore0);
    EXPECT_EQ(updatedScore0->getScore(), 15);  // 10 + 5
    auto removedIntent0 = registry->getComponent<ScoreIntentComponent>(entity0);
    EXPECT_FALSE(removedIntent0);

    // Entity 1: score unchanged, intent still present
    auto updatedScore1 = registry->getComponent<ScoreComponent>(entity1);
    ASSERT_TRUE(updatedScore1);
    EXPECT_EQ(updatedScore1->getScore(), 20);
    auto stillIntent1 = registry->getComponent<ScoreIntentComponent>(entity1);
    ASSERT_TRUE(stillIntent1);
    EXPECT_EQ(stillIntent1->getScore(), 10);
}

TEST_F(ScoreSystemTest, NegativeScoreIntent_ScoreDecreased) {
    // Create entity with ScoreTag, ScoreComponent, and negative ScoreIntent
    ecs::Entity entityId = 0;
    auto score = std::make_shared<ScoreComponent>(50);
    auto tag = std::make_shared<ScoreTag>();
    auto intent = std::make_shared<ScoreIntentComponent>(-10);

    registry->addComponent<ScoreTag>(entityId, tag);
    registry->addComponent<ScoreComponent>(entityId, score);
    registry->addComponent<ScoreIntentComponent>(entityId, intent);

    // Update
    scoreSystem->update(resourceManager, registry, 0.016f);

    // Check score decreased
    auto updatedScore = registry->getComponent<ScoreComponent>(entityId);
    ASSERT_TRUE(updatedScore);
    EXPECT_EQ(updatedScore->getScore(), 40);  // 50 - 10

    // Intent removed
    auto removedIntent = registry->getComponent<ScoreIntentComponent>(entityId);
    EXPECT_FALSE(removedIntent);
}