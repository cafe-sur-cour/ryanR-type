/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityPrefabManager Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../common/Prefab/APrefab.hpp"
#include "../../../common/ECS/entity/registry/ARegistry.hpp"

using namespace ecs;

/* EntityPrefabManager Tests */

TEST(EntityPrefabManagerTest, RegisterAndGetPrefab) {
    EntityPrefabManager manager;
    auto prefab = std::make_shared<APrefab>();
    std::string name = "testPrefab";

    manager.registerPrefab(name, prefab);
    auto retrieved = manager.getPrefab(name);

    EXPECT_EQ(retrieved, prefab);
}

TEST(EntityPrefabManagerTest, RegisterNullPrefab) {
    EntityPrefabManager manager;
    std::string name = "nullPrefab";

    manager.registerPrefab(name, nullptr);
    auto retrieved = manager.getPrefab(name);

    EXPECT_EQ(retrieved, nullptr);
}

TEST(EntityPrefabManagerTest, HasPrefab) {
    EntityPrefabManager manager;
    auto prefab = std::make_shared<APrefab>();
    std::string name = "existingPrefab";

    EXPECT_FALSE(manager.hasPrefab(name));

    manager.registerPrefab(name, prefab);
    EXPECT_TRUE(manager.hasPrefab(name));
}

TEST(EntityPrefabManagerTest, GetNonExistentPrefab) {
    EntityPrefabManager manager;
    std::string name = "nonExistent";

    auto retrieved = manager.getPrefab(name);
    EXPECT_EQ(retrieved, nullptr);
}

TEST(EntityPrefabManagerTest, CreateEntityFromPrefab) {
    EntityPrefabManager manager;
    auto registry = std::make_shared<ARegistry>();
    auto prefab = std::make_shared<APrefab>();
    std::string name = "createTest";

    manager.registerPrefab(name, prefab);
    size_t entityId = manager.createEntityFromPrefab(name, registry);

    EXPECT_NE(entityId, 0);
}

TEST(EntityPrefabManagerTest, CreateEntityFromNonExistentPrefab) {
    EntityPrefabManager manager;
    auto registry = std::make_shared<ARegistry>();
    std::string name = "nonExistent";

    size_t entityId = manager.createEntityFromPrefab(name, registry);

    EXPECT_EQ(entityId, 0); // As per implementation
}

TEST(EntityPrefabManagerTest, CreateEntityWithNullRegistry) {
    EntityPrefabManager manager;
    auto prefab = std::make_shared<APrefab>();
    std::string name = "nullRegistry";

    manager.registerPrefab(name, prefab);
    size_t entityId = manager.createEntityFromPrefab(name, nullptr);

    EXPECT_EQ(entityId, 0);
}

TEST(EntityPrefabManagerTest, DeletePrefab) {
    EntityPrefabManager manager;
    auto prefab = std::make_shared<APrefab>();
    std::string name = "deleteTest";

    manager.registerPrefab(name, prefab);
    EXPECT_TRUE(manager.hasPrefab(name));

    manager.deletePrefab(name);
    EXPECT_FALSE(manager.hasPrefab(name));
    EXPECT_EQ(manager.getPrefab(name), nullptr);
}

TEST(EntityPrefabManagerTest, DeleteNonExistentPrefab) {
    EntityPrefabManager manager;
    std::string name = "nonExistent";

    // Should not crash
    manager.deletePrefab(name);
}

TEST(EntityPrefabManagerTest, ClearPrefabs) {
    EntityPrefabManager manager;
    auto prefab1 = std::make_shared<APrefab>();
    auto prefab2 = std::make_shared<APrefab>();

    manager.registerPrefab("prefab1", prefab1);
    manager.registerPrefab("prefab2", prefab2);

    EXPECT_TRUE(manager.hasPrefab("prefab1"));
    EXPECT_TRUE(manager.hasPrefab("prefab2"));

    manager.clearPrefabs();

    EXPECT_FALSE(manager.hasPrefab("prefab1"));
    EXPECT_FALSE(manager.hasPrefab("prefab2"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}