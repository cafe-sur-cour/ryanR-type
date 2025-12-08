/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpawnSystem Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../../common/systems/spawn/SpawnSystem.hpp"
#include "../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../common/components/temporary/SpawnIntentComponent.hpp"
#include "../../../../common/components/permanent/TransformComponent.hpp"
#include "../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../common/Prefab/IPrefab.hpp"

using namespace ecs;

/* Mock Prefab for testing */
class MockPrefab : public IPrefab {
public:
    MockPrefab() = default;
    ~MockPrefab() = default;

    ecs::Entity instantiate(const std::shared_ptr<ecs::Registry> &registry) override {
        ecs::Entity newEntity = registry->createEntity();
        auto transform = std::make_shared<TransformComponent>();
        registry->addComponent<TransformComponent>(newEntity, transform);
        return newEntity;
    }
};

/* SpawnSystem Tests */

class SpawnSystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        registry = std::make_shared<Registry>();
        resourceManager = std::make_shared<ResourceManager>();
        spawnSystem = std::make_shared<SpawnSystem>();
        prefabManager = std::make_shared<EntityPrefabManager>();

        // Register components
        registry->registerComponent<SpawnIntentComponent>();
        registry->registerComponent<TransformComponent>();

        // Register prefab manager in resource manager
        resourceManager->add<EntityPrefabManager>(prefabManager);
    }

    std::shared_ptr<Registry> registry;
    std::shared_ptr<ResourceManager> resourceManager;
    std::shared_ptr<SpawnSystem> spawnSystem;
    std::shared_ptr<EntityPrefabManager> prefabManager;
};

TEST_F(SpawnSystemTest, EntityWithSpawnIntent_SpawnsNewEntity) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("test_prefab", mockPrefab);

    // Create entity with spawn intent
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(100.0f, 200.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("test_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update
    spawnSystem->update(resourceManager, registry, 0.016f);

    // Spawn intent should be removed from spawner
    EXPECT_FALSE(registry->hasComponent<SpawnIntentComponent>(spawnerEntity));

    // A new entity should have been created with TransformComponent at the spawn position
    auto view = registry->view<TransformComponent>();
    bool foundSpawnedEntity = false;
    for (auto entityId : view) {
        if (entityId != spawnerEntity) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            EXPECT_EQ(transform->getPosition().getX(), 100.0f);
            EXPECT_EQ(transform->getPosition().getY(), 200.0f);
            foundSpawnedEntity = true;
        }
    }
    EXPECT_TRUE(foundSpawnedEntity);
}

TEST_F(SpawnSystemTest, MultipleSpawnIntents_SpawnsMultipleEntities) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("test_prefab", mockPrefab);

    // Create first entity with spawn intent
    ecs::Entity spawner1 = registry->createEntity();
    auto spawnIntent1 = std::make_shared<SpawnIntentComponent>("test_prefab", math::Vector2f(10.0f, 20.0f));
    registry->addComponent<SpawnIntentComponent>(spawner1, spawnIntent1);

    // Create second entity with spawn intent
    ecs::Entity spawner2 = registry->createEntity();
    auto spawnIntent2 = std::make_shared<SpawnIntentComponent>("test_prefab", math::Vector2f(30.0f, 40.0f));
    registry->addComponent<SpawnIntentComponent>(spawner2, spawnIntent2);

    // Update
    spawnSystem->update(resourceManager, registry, 0.016f);

    // Both spawn intents should be removed
    EXPECT_FALSE(registry->hasComponent<SpawnIntentComponent>(spawner1));
    EXPECT_FALSE(registry->hasComponent<SpawnIntentComponent>(spawner2));

    // Two new entities should have been created
    auto view = registry->view<TransformComponent>();
    int spawnedCount = 0;
    for (auto entityId : view) {
        if (entityId != spawner1 && entityId != spawner2) {
            spawnedCount++;
        }
    }
    EXPECT_EQ(spawnedCount, 2);
}

TEST_F(SpawnSystemTest, SpawnIntent_PositionCorrectlySet) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("positioned_prefab", mockPrefab);

    // Create entity with spawn intent at specific position
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(500.0f, 300.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("positioned_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update
    spawnSystem->update(resourceManager, registry, 0.016f);

    // Find the spawned entity and verify position
    auto view = registry->view<TransformComponent>();
    for (auto entityId : view) {
        if (entityId != spawnerEntity) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            EXPECT_EQ(transform->getPosition().getX(), 500.0f);
            EXPECT_EQ(transform->getPosition().getY(), 300.0f);
        }
    }
}

TEST_F(SpawnSystemTest, SpawnIntent_NegativePosition) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("negative_pos_prefab", mockPrefab);

    // Create entity with spawn intent at negative position
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(-100.0f, -200.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("negative_pos_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update
    spawnSystem->update(resourceManager, registry, 0.016f);

    // Find the spawned entity and verify position
    auto view = registry->view<TransformComponent>();
    for (auto entityId : view) {
        if (entityId != spawnerEntity) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            EXPECT_EQ(transform->getPosition().getX(), -100.0f);
            EXPECT_EQ(transform->getPosition().getY(), -200.0f);
        }
    }
}

TEST_F(SpawnSystemTest, SpawnIntent_ZeroPosition) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("zero_pos_prefab", mockPrefab);

    // Create entity with spawn intent at zero position
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(0.0f, 0.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("zero_pos_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update
    spawnSystem->update(resourceManager, registry, 0.016f);

    // Find the spawned entity and verify position
    auto view = registry->view<TransformComponent>();
    for (auto entityId : view) {
        if (entityId != spawnerEntity) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            EXPECT_EQ(transform->getPosition().getX(), 0.0f);
            EXPECT_EQ(transform->getPosition().getY(), 0.0f);
        }
    }
}

TEST_F(SpawnSystemTest, InvalidPrefab_ThrowsException) {
    // Create entity with spawn intent for non-existent prefab
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(100.0f, 200.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("non_existent_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update should throw when prefab is not found
    EXPECT_THROW(spawnSystem->update(resourceManager, registry, 0.016f), std::exception);
}

TEST_F(SpawnSystemTest, DeltaTimeIgnored) {
    // Register a mock prefab
    auto mockPrefab = std::make_shared<MockPrefab>();
    prefabManager->registerPrefab("test_prefab", mockPrefab);

    // Create entity with spawn intent
    ecs::Entity spawnerEntity = registry->createEntity();
    math::Vector2f spawnPos(100.0f, 200.0f);
    auto spawnIntent = std::make_shared<SpawnIntentComponent>("test_prefab", spawnPos);
    registry->addComponent<SpawnIntentComponent>(spawnerEntity, spawnIntent);

    // Update with different delta times - behavior should be the same
    spawnSystem->update(resourceManager, registry, 0.0f);

    // Spawn intent should be removed regardless of delta time
    EXPECT_FALSE(registry->hasComponent<SpawnIntentComponent>(spawnerEntity));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
