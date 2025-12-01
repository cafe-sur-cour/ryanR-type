/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerPrefab Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../common/Prefab/PlayerPrefab/PlayerPrefab.hpp"
#include "../../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../common/components/permanent/SpeedComponent.hpp"
#include "../../../client/components/rendering/AnimationComponent.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"

using namespace ecs;

/* PlayerPrefab Tests */

TEST(PlayerPrefabTest, InstantiateCreatesEntityWithComponents) {
    auto registry = std::make_shared<ARegistry>();

    // Register components
    registry->registerComponent<TransformComponent>();
    registry->registerComponent<VelocityComponent>();
    registry->registerComponent<SpeedComponent>();
    registry->registerComponent<AnimationComponent>();
    registry->registerComponent<PlayerTag>();

    PlayerPrefab prefab(100.0f, 200.0f, 1.5f, "assets/player.png", 64.0f, 64.0f, 0.0f, 0.0f, 4);

    ecs::Entity entityId = prefab.instantiate(registry);

    EXPECT_NE(entityId, 0);

    // Check components are added
    EXPECT_TRUE(registry->hasComponent<TransformComponent>(entityId));
    EXPECT_TRUE(registry->hasComponent<VelocityComponent>(entityId));
    EXPECT_TRUE(registry->hasComponent<SpeedComponent>(entityId));
    EXPECT_TRUE(registry->hasComponent<AnimationComponent>(entityId));
    EXPECT_TRUE(registry->hasComponent<PlayerTag>(entityId));

    // Check TransformComponent values
    auto transform = registry->getComponent<TransformComponent>(entityId);
    EXPECT_FLOAT_EQ(transform->getPosition().getX(), 100.0f);
    EXPECT_FLOAT_EQ(transform->getPosition().getY(), 200.0f);
    EXPECT_FLOAT_EQ(transform->getScale().getX(), 1.5f);
    EXPECT_FLOAT_EQ(transform->getScale().getY(), 1.5f);

    // Check AnimationComponent values
    auto animation = registry->getComponent<AnimationComponent>(entityId);
    EXPECT_EQ(animation->getTexturePath(), "assets/player.png");
    EXPECT_FLOAT_EQ(animation->getFrameWidth(), 64.0f);
    EXPECT_FLOAT_EQ(animation->getFrameHeight(), 64.0f);
    EXPECT_EQ(animation->getFrameCount(), 4);
    EXPECT_FLOAT_EQ(animation->getStartWidth(), 0.0f);
    EXPECT_FLOAT_EQ(animation->getStartHeight(), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}