/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpawnIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/SpawnIntentComponent.hpp"

using namespace ecs;

/* SpawnIntentComponent Tests */

TEST(SpawnIntentComponentTest, ParameterizedConstructor) {
    math::Vector2f pos(100.0f, 200.0f);
    SpawnIntentComponent comp("enemy_ship", pos);
    EXPECT_EQ(comp.getPrefabName(), "enemy_ship");
    EXPECT_EQ(comp.getPosition().getX(), 100.0f);
    EXPECT_EQ(comp.getPosition().getY(), 200.0f);
}

TEST(SpawnIntentComponentTest, GettersAndSetters_PrefabName) {
    math::Vector2f pos(0.0f, 0.0f);
    SpawnIntentComponent comp("initial_prefab", pos);

    comp.setPrefabName("new_prefab");
    EXPECT_EQ(comp.getPrefabName(), "new_prefab");
}

TEST(SpawnIntentComponentTest, GettersAndSetters_Position) {
    math::Vector2f pos(0.0f, 0.0f);
    SpawnIntentComponent comp("prefab", pos);

    math::Vector2f newPos(50.0f, 75.0f);
    comp.setPosition(newPos);
    EXPECT_EQ(comp.getPosition().getX(), 50.0f);
    EXPECT_EQ(comp.getPosition().getY(), 75.0f);
}

TEST(SpawnIntentComponentTest, EmptyPrefabName) {
    math::Vector2f pos(10.0f, 20.0f);
    SpawnIntentComponent comp("", pos);
    EXPECT_EQ(comp.getPrefabName(), "");
    EXPECT_EQ(comp.getPosition().getX(), 10.0f);
    EXPECT_EQ(comp.getPosition().getY(), 20.0f);
}

TEST(SpawnIntentComponentTest, NegativePosition) {
    math::Vector2f pos(-100.0f, -200.0f);
    SpawnIntentComponent comp("test_prefab", pos);
    EXPECT_EQ(comp.getPosition().getX(), -100.0f);
    EXPECT_EQ(comp.getPosition().getY(), -200.0f);
}

TEST(SpawnIntentComponentTest, ZeroPosition) {
    math::Vector2f pos(0.0f, 0.0f);
    SpawnIntentComponent comp("zero_prefab", pos);
    EXPECT_EQ(comp.getPosition().getX(), 0.0f);
    EXPECT_EQ(comp.getPosition().getY(), 0.0f);
}

TEST(SpawnIntentComponentTest, MultipleUpdates) {
    math::Vector2f pos(1.0f, 1.0f);
    SpawnIntentComponent comp("prefab1", pos);

    comp.setPrefabName("prefab2");
    comp.setPosition(math::Vector2f(2.0f, 2.0f));

    comp.setPrefabName("prefab3");
    comp.setPosition(math::Vector2f(3.0f, 3.0f));

    EXPECT_EQ(comp.getPrefabName(), "prefab3");
    EXPECT_EQ(comp.getPosition().getX(), 3.0f);
    EXPECT_EQ(comp.getPosition().getY(), 3.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
