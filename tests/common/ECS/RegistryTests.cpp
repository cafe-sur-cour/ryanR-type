/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Registry Tests
*/

#include <gtest/gtest.h>
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/components/temporary/MovementIntentComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"

using namespace ecs;

/* Registry Tests */

TEST(ARegistryTest, GetMaxEntityId_EmptyRegistry) {
    auto registry = std::make_shared<Registry>();
    EXPECT_EQ(registry->getMaxEntityId(), 0);
}

TEST(ARegistryTest, GetMaxEntityId_WithComponents) {
    auto registry = std::make_shared<Registry>();

    // Add components at different entity IDs
    registry->registerComponent<PlayerTag>();
    registry->registerComponent<TransformComponent>();

    auto playerTag = std::make_shared<PlayerTag>();
    auto transform = std::make_shared<TransformComponent>();

    registry->addComponent(0, playerTag);
    registry->addComponent(5, transform);
    registry->addComponent(3, playerTag);

    // Max entity ID should be 5 (highest index used)
    EXPECT_EQ(registry->getMaxEntityId(), 6); // size() returns size, not max index
}

TEST(ARegistryTest, GetMaxEntityId_MultipleComponentTypes) {
    auto registry = std::make_shared<Registry>();

    registry->registerComponent<PlayerTag>();
    registry->registerComponent<MovementIntentComponent>();
    registry->registerComponent<TransformComponent>();

    // Add components at entity ID 10
    auto playerTag = std::make_shared<PlayerTag>();
    auto movementIntent = std::make_shared<MovementIntentComponent>();
    auto transform = std::make_shared<TransformComponent>();

    registry->addComponent(10, playerTag);
    registry->addComponent(10, movementIntent);
    registry->addComponent(10, transform);

    EXPECT_EQ(registry->getMaxEntityId(), 11); // size() returns size, not max index
}

TEST(ARegistryTest, View_EmptyRegistry) {
    auto registry = std::make_shared<Registry>();
    auto view = registry->view<PlayerTag>();

    // Should iterate over no entities
    int count = 0;
    for (auto entityId : view) {
        (void)entityId;
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(ARegistryTest, View_SingleEntity) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<PlayerTag>();

    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    auto view = registry->view<PlayerTag>();

    int count = 0;
    size_t foundEntityId = 0;
    for (auto entityId : view) {
        count++;
        foundEntityId = entityId;
    }
    EXPECT_EQ(count, 1);
    EXPECT_EQ(foundEntityId, 0);
}

TEST(ARegistryTest, View_MultipleEntities) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<PlayerTag>();

    auto playerTag1 = std::make_shared<PlayerTag>();
    auto playerTag2 = std::make_shared<PlayerTag>();
    auto playerTag3 = std::make_shared<PlayerTag>();

    registry->addComponent(1, playerTag1);
    registry->addComponent(5, playerTag2);
    registry->addComponent(10, playerTag3);

    auto view = registry->view<PlayerTag>();

    std::vector<size_t> foundEntities;
    for (auto entityId : view) {
        foundEntities.push_back(entityId);
    }

    EXPECT_EQ(foundEntities.size(), 3);
    EXPECT_EQ(foundEntities[0], 1);
    EXPECT_EQ(foundEntities[1], 5);
    EXPECT_EQ(foundEntities[2], 10);
}

TEST(ARegistryTest, View_MultipleComponents) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<PlayerTag>();
    registry->registerComponent<TransformComponent>();

    // ecs::Entity 0: PlayerTag only
    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    // ecs::Entity 1: PlayerTag + TransformComponent
    auto playerTag2 = std::make_shared<PlayerTag>();
    auto transform = std::make_shared<TransformComponent>();
    registry->addComponent(1, playerTag2);
    registry->addComponent(1, transform);

    // ecs::Entity 2: TransformComponent only
    auto transform2 = std::make_shared<TransformComponent>();
    registry->addComponent(2, transform2);

    auto view = registry->view<PlayerTag, TransformComponent>();

    std::vector<size_t> foundEntities;
    for (auto entityId : view) {
        foundEntities.push_back(entityId);
    }

    EXPECT_EQ(foundEntities.size(), 1);
    EXPECT_EQ(foundEntities[0], 1);
}

TEST(ARegistryTest, Group_BasicFunctionality) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<PlayerTag>();

    auto playerTag = std::make_shared<PlayerTag>();
    registry->addComponent(0, playerTag);

    auto group = registry->group<PlayerTag>();

    int count = 0;
    size_t foundEntityId = 0;
    for (auto entityId : group) {
        count++;
        foundEntityId = entityId;
    }
    EXPECT_EQ(count, 1);
    EXPECT_EQ(foundEntityId, 0);
}

TEST(ARegistryTest, GetComponents_SingleComponent) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<TransformComponent>();

    auto transform = std::make_shared<TransformComponent>();
    registry->addComponent(0, transform);

    auto components = registry->getComponents<TransformComponent>(0);
    EXPECT_EQ(components.size(), 1);
    EXPECT_EQ(components[0], transform);
}

TEST(ARegistryTest, GetComponents_MultipleComponentsSameType) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<TransformComponent>();

    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(10.0f, 20.0f));
    auto transform2 = std::make_shared<TransformComponent>(math::Vector2f(30.0f, 40.0f));
    registry->addComponent(0, transform1);
    registry->addComponent(0, transform2);

    auto components = registry->getComponents<TransformComponent>(0);
    EXPECT_EQ(components.size(), 2);
    EXPECT_EQ(components[0], transform1);
    EXPECT_EQ(components[1], transform2);
}

TEST(ARegistryTest, GetComponents_NoComponents) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<TransformComponent>();

    auto components = registry->getComponents<TransformComponent>(0);
    EXPECT_EQ(components.size(), 0);
}

TEST(ARegistryTest, GetComponent_ReturnsFirstWhenMultiple) {
    auto registry = std::make_shared<Registry>();
    registry->registerComponent<TransformComponent>();

    auto transform1 = std::make_shared<TransformComponent>(math::Vector2f(10.0f, 20.0f));
    auto transform2 = std::make_shared<TransformComponent>(math::Vector2f(30.0f, 40.0f));
    registry->addComponent(0, transform1);
    registry->addComponent(0, transform2);

    auto firstComponent = registry->getComponent<TransformComponent>(0);
    EXPECT_EQ(firstComponent, transform1);  // Should return the first one
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}