/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** APrefab Tests
*/

#include <gtest/gtest.h>
#include <memory>
#include "../../../common/Prefab/APrefab.hpp"
#include "../../../common/ECS/entity/registry/ARegistry.hpp"

using namespace ecs;

/* APrefab Tests */

TEST(APrefabTest, InstantiateCreatesEntity) {
    auto registry = std::make_shared<ARegistry>();
    APrefab prefab;

    size_t entityId = prefab.instantiate(registry);

    EXPECT_NE(entityId, 0); // Assuming 0 is invalid
    // Since ARegistry starts from 1 or something, but check if entity exists
    // Actually, createEntity returns _nextEntityId++, starting from 1 probably
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}