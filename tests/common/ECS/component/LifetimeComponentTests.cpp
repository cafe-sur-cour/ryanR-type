/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** LifetimeComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/LifetimeComponent.hpp"

using namespace ecs;

/* LifetimeComponent Tests */

TEST(LifetimeComponentTest, DefaultConstructor) {
    LifetimeComponent comp;
    EXPECT_FLOAT_EQ(comp.getLifetime(), 0.0f);
}

TEST(LifetimeComponentTest, ParameterizedConstructor) {
    LifetimeComponent comp(5.0f);
    EXPECT_FLOAT_EQ(comp.getLifetime(), 5.0f);
}

TEST(LifetimeComponentTest, GetLifetime) {
    LifetimeComponent comp(10.5f);
    EXPECT_FLOAT_EQ(comp.getLifetime(), 10.5f);
}

TEST(LifetimeComponentTest, SetLifetime) {
    LifetimeComponent comp;
    comp.setLifetime(7.25f);
    EXPECT_FLOAT_EQ(comp.getLifetime(), 7.25f);
}

TEST(LifetimeComponentTest, SetLifetimeNegative) {
    LifetimeComponent comp;
    comp.setLifetime(-1.0f);
    EXPECT_FLOAT_EQ(comp.getLifetime(), -1.0f);
}

TEST(LifetimeComponentTest, SetLifetimeZero) {
    LifetimeComponent comp(5.0f);
    comp.setLifetime(0.0f);
    EXPECT_FLOAT_EQ(comp.getLifetime(), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}