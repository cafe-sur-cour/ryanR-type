/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/ShootIntentComponent.hpp"

using namespace ecs;

/* ShootIntentComponent Tests */

TEST(ShootIntentComponentTest, DefaultConstructor) {
    ShootIntentComponent comp;
    EXPECT_FLOAT_EQ(comp.getAngle(), 0.0f);
}

TEST(ShootIntentComponentTest, ParameterizedConstructor) {
    ShootIntentComponent comp(45.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 45.0f);
}

TEST(ShootIntentComponentTest, SetAngle) {
    ShootIntentComponent comp;
    comp.setAngle(90.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 90.0f);
}

TEST(ShootIntentComponentTest, UpdateAngle) {
    ShootIntentComponent comp(30.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 30.0f);
    
    comp.setAngle(60.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 60.0f);
    
    comp.setAngle(120.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 120.0f);
}

TEST(ShootIntentComponentTest, NegativeAngle) {
    ShootIntentComponent comp(-45.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), -45.0f);
    
    comp.setAngle(-90.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), -90.0f);
}

TEST(ShootIntentComponentTest, LargeAngle) {
    ShootIntentComponent comp(360.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 360.0f);
    
    comp.setAngle(720.0f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 720.0f);
}

TEST(ShootIntentComponentTest, FloatPrecision) {
    ShootIntentComponent comp(12.345f);
    EXPECT_FLOAT_EQ(comp.getAngle(), 12.345f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
