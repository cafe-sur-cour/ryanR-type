/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/DamageComponent.hpp"

using namespace ecs;

/* DamageComponent Tests */

TEST(DamageComponentTest, DefaultConstructor) {
    DamageComponent comp;
    EXPECT_FLOAT_EQ(comp.getDamage(), 0.0f);
}

TEST(DamageComponentTest, ParameterizedConstructor) {
    float damage = 25.0f;
    DamageComponent comp(damage);
    EXPECT_FLOAT_EQ(comp.getDamage(), 25.0f);
}

TEST(DamageComponentTest, SetDamage) {
    DamageComponent comp;
    comp.setDamage(50.0f);
    EXPECT_FLOAT_EQ(comp.getDamage(), 50.0f);
}

TEST(DamageComponentTest, NegativeDamage) {
    DamageComponent comp(-10.0f);
    EXPECT_FLOAT_EQ(comp.getDamage(), -10.0f);
}

TEST(DamageComponentTest, FloatPrecision) {
    DamageComponent comp(12.345f);
    EXPECT_FLOAT_EQ(comp.getDamage(), 12.345f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}