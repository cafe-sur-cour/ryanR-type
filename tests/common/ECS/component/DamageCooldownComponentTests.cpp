/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageCooldownComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/DamageCooldownComponent.hpp"

using namespace ecs;

/* DamageCooldownComponent Tests */

TEST(DamageCooldownComponentTest, DefaultConstructor) {
    DamageCooldownComponent comp;
    EXPECT_FLOAT_EQ(comp.getCooldown(), constants::TRIGGER_DAMAGE_COOLDOWN);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 0.0f);
}

TEST(DamageCooldownComponentTest, ParameterizedConstructor) {
    DamageCooldownComponent comp(1.5f);
    EXPECT_FLOAT_EQ(comp.getCooldown(), 1.5f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 0.0f);
}

TEST(DamageCooldownComponentTest, SetCooldown) {
    DamageCooldownComponent comp;
    comp.setCooldown(2.5f);
    EXPECT_FLOAT_EQ(comp.getCooldown(), 2.5f);
}

TEST(DamageCooldownComponentTest, SetLastDamageTime) {
    DamageCooldownComponent comp;
    comp.setLastDamageTime(10.5f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 10.5f);
}

TEST(DamageCooldownComponentTest, UpdateLastDamageTime) {
    DamageCooldownComponent comp(1.0f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 0.0f);
    
    comp.setLastDamageTime(5.0f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 5.0f);
    
    comp.setLastDamageTime(10.0f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 10.0f);
}

TEST(DamageCooldownComponentTest, NegativeValues) {
    DamageCooldownComponent comp(-1.0f);
    EXPECT_FLOAT_EQ(comp.getCooldown(), -1.0f);
    
    comp.setLastDamageTime(-5.0f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), -5.0f);
}

TEST(DamageCooldownComponentTest, FloatPrecision) {
    DamageCooldownComponent comp(1.234f);
    EXPECT_FLOAT_EQ(comp.getCooldown(), 1.234f);
    
    comp.setLastDamageTime(5.678f);
    EXPECT_FLOAT_EQ(comp.getLastDamageTime(), 5.678f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
