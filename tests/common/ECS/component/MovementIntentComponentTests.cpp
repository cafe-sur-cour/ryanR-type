/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MovementIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/MovementIntentComponent.hpp"

using namespace ecs;

/* MovementIntentComponent Tests */

TEST(MovementIntentComponentTest, DefaultConstructor) {
    MovementIntentComponent comp;
    math::Vector2f expected(0.0f, 0.0f);
    EXPECT_EQ(comp.getDirection().getX(), expected.getX());
    EXPECT_EQ(comp.getDirection().getY(), expected.getY());
    EXPECT_FALSE(comp.isActive());
}

TEST(MovementIntentComponentTest, ParameterizedConstructor) {
    math::Vector2f dir(1.0f, -2.0f);
    MovementIntentComponent comp(dir, true);
    EXPECT_EQ(comp.getDirection().getX(), 1.0f);
    EXPECT_EQ(comp.getDirection().getY(), -2.0f);
    EXPECT_TRUE(comp.isActive());
}

TEST(MovementIntentComponentTest, GettersAndSetters) {
    MovementIntentComponent comp;
    math::Vector2f newDir(3.0f, 4.0f);
    comp.setDirection(newDir);
    comp.setActive(true);
    EXPECT_EQ(comp.getDirection().getX(), 3.0f);
    EXPECT_EQ(comp.getDirection().getY(), 4.0f);
    EXPECT_TRUE(comp.isActive());
}

TEST(MovementIntentComponentTest, InheritsFromAComponent) {
    MovementIntentComponent comp;
    // Should inherit from IComponent
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
