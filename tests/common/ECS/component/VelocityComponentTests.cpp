/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** VelocityComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/VelocityComponent.hpp"

using namespace ecs;

/* VelocityComponent Tests */

TEST(VelocityComponentTest, DefaultConstructor) {
    VelocityComponent comp;
    math::Vector2f expected(0.0f, 0.0f);
    EXPECT_EQ(comp.getVelocity().getX(), expected.getX());
    EXPECT_EQ(comp.getVelocity().getY(), expected.getY());
}

TEST(VelocityComponentTest, ParameterizedConstructor) {
    math::Vector2f vel(1.0f, -2.0f);
    VelocityComponent comp(vel);
    EXPECT_EQ(comp.getVelocity().getX(), 1.0f);
    EXPECT_EQ(comp.getVelocity().getY(), -2.0f);
}

TEST(VelocityComponentTest, GettersAndSetters) {
    VelocityComponent comp;
    math::Vector2f newVel(3.0f, 4.0f);
    comp.setVelocity(newVel);
    EXPECT_EQ(comp.getVelocity().getX(), 3.0f);
    EXPECT_EQ(comp.getVelocity().getY(), 4.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}