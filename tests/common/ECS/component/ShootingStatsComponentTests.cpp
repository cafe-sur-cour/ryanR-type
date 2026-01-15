/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootingStatsComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ShootingStatsComponent.hpp"

using namespace ecs;

/* ShootingStatsComponent Tests */

TEST(ShootingStatsComponentTest, DefaultConstructor) {
    ShootingStatsComponent comp;
    EXPECT_FLOAT_EQ(comp.getFireRate(), 1.0f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 0.0f);
    
    MultiShotPattern pattern = comp.getMultiShotPattern();
    EXPECT_EQ(pattern.shotCount, 1);
    EXPECT_FLOAT_EQ(pattern.angleSpread, 0.0f);
    EXPECT_FLOAT_EQ(pattern.offsetDistance, 0.0f);
    EXPECT_FLOAT_EQ(pattern.angleOffset, 0.0f);
}

TEST(ShootingStatsComponentTest, ParameterizedConstructor) {
    MultiShotPattern customPattern;
    customPattern.shotCount = 3;
    customPattern.angleSpread = 15.0f;
    customPattern.offsetDistance = 10.0f;
    customPattern.angleOffset = 5.0f;
    
    ShootingStatsComponent comp(2.5f, customPattern);
    EXPECT_FLOAT_EQ(comp.getFireRate(), 2.5f);
    
    MultiShotPattern pattern = comp.getMultiShotPattern();
    EXPECT_EQ(pattern.shotCount, 3);
    EXPECT_FLOAT_EQ(pattern.angleSpread, 15.0f);
    EXPECT_FLOAT_EQ(pattern.offsetDistance, 10.0f);
    EXPECT_FLOAT_EQ(pattern.angleOffset, 5.0f);
}

TEST(ShootingStatsComponentTest, SetFireRate) {
    ShootingStatsComponent comp;
    comp.setFireRate(3.0f);
    EXPECT_FLOAT_EQ(comp.getFireRate(), 3.0f);
}

TEST(ShootingStatsComponentTest, SetCooldownTimer) {
    ShootingStatsComponent comp;
    comp.setCooldownTimer(1.5f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 1.5f);
}

TEST(ShootingStatsComponentTest, SetMultiShotPattern) {
    ShootingStatsComponent comp;
    
    MultiShotPattern newPattern;
    newPattern.shotCount = 5;
    newPattern.angleSpread = 30.0f;
    newPattern.offsetDistance = 20.0f;
    newPattern.angleOffset = 10.0f;
    
    comp.setMultiShotPattern(newPattern);
    
    MultiShotPattern pattern = comp.getMultiShotPattern();
    EXPECT_EQ(pattern.shotCount, 5);
    EXPECT_FLOAT_EQ(pattern.angleSpread, 30.0f);
    EXPECT_FLOAT_EQ(pattern.offsetDistance, 20.0f);
    EXPECT_FLOAT_EQ(pattern.angleOffset, 10.0f);
}

TEST(ShootingStatsComponentTest, UpdateCooldownTimer) {
    ShootingStatsComponent comp;
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 0.0f);
    
    comp.setCooldownTimer(0.5f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 0.5f);
    
    comp.setCooldownTimer(1.0f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), 1.0f);
}

TEST(ShootingStatsComponentTest, NegativeValues) {
    ShootingStatsComponent comp(-1.0f);
    EXPECT_FLOAT_EQ(comp.getFireRate(), -1.0f);
    
    comp.setCooldownTimer(-2.0f);
    EXPECT_FLOAT_EQ(comp.getCooldownTimer(), -2.0f);
}

TEST(ShootingStatsComponentTest, MultiShotPatternEdgeCases) {
    MultiShotPattern pattern;
    pattern.shotCount = 0;
    pattern.angleSpread = -45.0f;
    pattern.offsetDistance = -5.0f;
    pattern.angleOffset = -90.0f;
    
    ShootingStatsComponent comp(1.0f, pattern);
    MultiShotPattern retrieved = comp.getMultiShotPattern();
    
    EXPECT_EQ(retrieved.shotCount, 0);
    EXPECT_FLOAT_EQ(retrieved.angleSpread, -45.0f);
    EXPECT_FLOAT_EQ(retrieved.offsetDistance, -5.0f);
    EXPECT_FLOAT_EQ(retrieved.angleOffset, -90.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
