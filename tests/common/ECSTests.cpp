/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Tests
*/

#include <gtest/gtest.h>
#include "../../common/components/base/IComponent.hpp"
#include "../../common/ECS/entity/Entity.hpp"
#include "../../common/ECS/entity/registry/Registry.hpp"

using namespace ecs;

/* Registry Tests */

class TestComponent : public IComponent {
public:
    TestComponent(int value) : _value(value) {}
    int getValue() const { return _value; }
private:
    int _value;
};

TEST(ARegistryTest, RegisterComponent) {
    Registry registry;
    registry.registerComponent<TestComponent>();
    // Should not crash
}

TEST(ARegistryTest, AddGetComponent) {
    Registry registry;
    registry.registerComponent<TestComponent>();

    ecs::Entity entityId = 1;
    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);

    auto retrieved = registry.getComponent<TestComponent>(entityId);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->getValue(), 42);
}

TEST(ARegistryTest, HasComponent) {
    Registry registry;
    registry.registerComponent<TestComponent>();

    ecs::Entity entityId = 1;
    EXPECT_FALSE(registry.hasComponent<TestComponent>(entityId));

    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);
    EXPECT_TRUE(registry.hasComponent<TestComponent>(entityId));
}

TEST(ARegistryTest, RemoveComponent) {
    Registry registry;
    registry.registerComponent<TestComponent>();

    ecs::Entity entityId = 1;
    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);
    EXPECT_TRUE(registry.hasComponent<TestComponent>(entityId));

    registry.removeAllComponents<TestComponent>(entityId);
    EXPECT_FALSE(registry.hasComponent<TestComponent>(entityId));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
