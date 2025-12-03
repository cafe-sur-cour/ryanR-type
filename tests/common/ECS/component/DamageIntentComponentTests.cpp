/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/DamageIntentComponent.hpp"

using namespace ecs;

/* DamageIntentComponent Tests */

TEST(DamageIntentComponentTest, DefaultConstructor) {
    DamageIntentComponent comp;
    EXPECT_FLOAT_EQ(comp.getDamages(), 0.0f);
}

TEST(DamageIntentComponentTest, ParameterizedConstructor) {
    DamageIntentComponent comp(25.0f);
    EXPECT_FLOAT_EQ(comp.getDamages(), 25.0f);
}

TEST(DamageIntentComponentTest, GetDamages) {
    DamageIntentComponent comp(50.0f);
    EXPECT_FLOAT_EQ(comp.getDamages(), 50.0f);
}

TEST(DamageIntentComponentTest, SetDamages) {
    DamageIntentComponent comp;
    comp.setDamages(75.0f);
    EXPECT_FLOAT_EQ(comp.getDamages(), 75.0f);
}

TEST(DamageIntentComponentTest, NegativeDamages) {
    DamageIntentComponent comp(-10.0f);
    EXPECT_FLOAT_EQ(comp.getDamages(), -10.0f);
}

TEST(DamageIntentComponentTest, ZeroDamages) {
    DamageIntentComponent comp(0.0f);
    EXPECT_FLOAT_EQ(comp.getDamages(), 0.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}