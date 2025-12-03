/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/HealthComponent.hpp"

using namespace ecs;

/* HealthComponent Tests */

TEST(HealthComponentTest, DefaultConstructor) {
    HealthComponent comp;
    EXPECT_FLOAT_EQ(comp.getHealth(), 100.0f);
}

TEST(HealthComponentTest, ParameterizedConstructor) {
    HealthComponent comp(50.0f);
    EXPECT_FLOAT_EQ(comp.getHealth(), 50.0f);
}

TEST(HealthComponentTest, GetHealth) {
    HealthComponent comp(75.0f);
    EXPECT_FLOAT_EQ(comp.getHealth(), 75.0f);
}

TEST(HealthComponentTest, SetHealth) {
    HealthComponent comp;
    comp.setHealth(25.0f);
    EXPECT_FLOAT_EQ(comp.getHealth(), 25.0f);
}

TEST(HealthComponentTest, NegativeHealth) {
    HealthComponent comp(-10.0f);
    EXPECT_FLOAT_EQ(comp.getHealth(), -10.0f);
}

TEST(HealthComponentTest, ZeroHealth) {
    HealthComponent comp(0.0f);
    EXPECT_FLOAT_EQ(comp.getHealth(), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}