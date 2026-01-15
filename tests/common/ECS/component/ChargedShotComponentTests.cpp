/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ChargedShotComponent.hpp"

using namespace ecs;

/* ChargedShotComponent Tests */

TEST(ChargedShotComponentTest, DefaultConstructor) {
    ChargedShotComponent comp;
    EXPECT_FLOAT_EQ(comp.getCharge(), 0.0f);
    EXPECT_FLOAT_EQ(comp.getMaxCharge(), 0.0f);
    EXPECT_FLOAT_EQ(comp.getReloadTime(), 0.0f);
}

TEST(ChargedShotComponentTest, ParameterizedConstructor) {
    ChargedShotComponent comp(50.0f, 100.0f, 2.0f);
    EXPECT_FLOAT_EQ(comp.getCharge(), 50.0f);
    EXPECT_FLOAT_EQ(comp.getMaxCharge(), 100.0f);
    EXPECT_FLOAT_EQ(comp.getReloadTime(), 2.0f);
}

TEST(ChargedShotComponentTest, SetCharge) {
    ChargedShotComponent comp;
    comp.setCharge(75.0f);
    EXPECT_FLOAT_EQ(comp.getCharge(), 75.0f);
}

TEST(ChargedShotComponentTest, SetMaxCharge) {
    ChargedShotComponent comp;
    comp.setMaxCharge(150.0f);
    EXPECT_FLOAT_EQ(comp.getMaxCharge(), 150.0f);
}

TEST(ChargedShotComponentTest, SetReloadTime) {
    ChargedShotComponent comp;
    comp.setReloadTime(3.5f);
    EXPECT_FLOAT_EQ(comp.getReloadTime(), 3.5f);
}

TEST(ChargedShotComponentTest, NegativeValues) {
    ChargedShotComponent comp(-10.0f, -50.0f, -1.0f);
    EXPECT_FLOAT_EQ(comp.getCharge(), -10.0f);
    EXPECT_FLOAT_EQ(comp.getMaxCharge(), -50.0f);
    EXPECT_FLOAT_EQ(comp.getReloadTime(), -1.0f);
}

TEST(ChargedShotComponentTest, FloatPrecision) {
    ChargedShotComponent comp(12.345f, 67.890f, 1.234f);
    EXPECT_FLOAT_EQ(comp.getCharge(), 12.345f);
    EXPECT_FLOAT_EQ(comp.getMaxCharge(), 67.890f);
    EXPECT_FLOAT_EQ(comp.getReloadTime(), 1.234f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
