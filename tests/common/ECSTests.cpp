/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ECS Tests
*/

#include <gtest/gtest.h>
#include "../../common/ECS/component/AComponent.hpp"
#include "../../common/ECS/entity/AEntity.hpp"
#include "../../common/ECS/system/ASystem.hpp"
#include "../../common/ECS/system/systemManager/ASystemManager.hpp"
#include "../../common/ECS/entity/registry/ARegistry.hpp"
#include "../../common/ECS/context/AContext.hpp"

using namespace ecs;

/* AComponent Tests */

TEST(AComponentTest, DefaultState) {
    AComponent comp;
    EXPECT_EQ(comp.getState(), ComponentState::Permanent);
}

TEST(AComponentTest, SetState) {
    AComponent comp;
    comp.setState(ComponentState::Temporary);
    EXPECT_EQ(comp.getState(), ComponentState::Temporary);
}

/* AEntity Tests */

TEST(AEntityTest, OperatorSizeT) {
    AEntity e;
    size_t id = e;
    EXPECT_GE(id, 0);
}

/* ASystemManager Tests */

class TestSystem : public ASystem {
protected:
    void update(std::shared_ptr<AContext> context, float deltaTime) override {
        // Do nothing for test
    }
};

TEST(ASystemManagerTest, AddAndUpdateSystems) {
    ASystemManager manager;
    auto system = std::make_shared<TestSystem>();
    manager.addSystem(std::static_pointer_cast<ISystem>(system));

    auto context = std::make_shared<AContext>();
    manager.updateAllSystems(context, 0.1f);  // Should not crash
}

TEST(ASystemManagerTest, RemoveSystem) {
    ASystemManager manager;
    auto system = std::make_shared<TestSystem>();
    manager.addSystem(std::static_pointer_cast<ISystem>(system));
    manager.removeSystem(std::static_pointer_cast<ISystem>(system));

    auto context = std::make_shared<AContext>();
    manager.updateAllSystems(context, 0.1f);  // Should not crash
}

/* ARegistry Tests */

class TestComponent : public AComponent {
public:
    TestComponent(int value) : _value(value) {}
    int getValue() const { return _value; }
private:
    int _value;
};

TEST(ARegistryTest, RegisterComponent) {
    ARegistry registry;
    registry.registerComponent<TestComponent>();
    // Should not crash
}

TEST(ARegistryTest, AddGetComponent) {
    ARegistry registry;
    registry.registerComponent<TestComponent>();

    int entityId = 1;
    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);

    auto retrieved = registry.getComponent<TestComponent>(entityId);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->getValue(), 42);
}

TEST(ARegistryTest, HasComponent) {
    ARegistry registry;
    registry.registerComponent<TestComponent>();

    int entityId = 1;
    EXPECT_FALSE(registry.hasComponent<TestComponent>(entityId));

    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);
    EXPECT_TRUE(registry.hasComponent<TestComponent>(entityId));
}

TEST(ARegistryTest, RemoveComponent) {
    ARegistry registry;
    registry.registerComponent<TestComponent>();

    int entityId = 1;
    auto comp = std::make_shared<TestComponent>(42);
    registry.addComponent<TestComponent>(entityId, comp);
    EXPECT_TRUE(registry.hasComponent<TestComponent>(entityId));

    registry.removeComponent<TestComponent>(entityId);
    EXPECT_FALSE(registry.hasComponent<TestComponent>(entityId));
}

TEST(ARegistryTest, RemoveAllComponentsWithState) {
    ARegistry registry;
    registry.registerComponent<TestComponent>();

    int entityId = 1;
    auto comp = std::make_shared<TestComponent>(42);
    comp->setState(ComponentState::Temporary);
    registry.addComponent<TestComponent>(entityId, comp);

    registry.removeAllComponentsWithState(ComponentState::Temporary);
    EXPECT_FALSE(registry.hasComponent<TestComponent>(entityId));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}