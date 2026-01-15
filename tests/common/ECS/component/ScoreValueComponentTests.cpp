/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreValueComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ScoreValueComponent.hpp"

using namespace ecs;

/* ScoreValueComponent Tests */

TEST(ScoreValueComponentTest, DefaultConstructor) {
    ScoreValueComponent comp;
    EXPECT_EQ(comp.getScoreValue(), 0);
}

TEST(ScoreValueComponentTest, ParameterizedConstructor) {
    ScoreValueComponent comp(100);
    EXPECT_EQ(comp.getScoreValue(), 100);
}

TEST(ScoreValueComponentTest, SetScoreValue) {
    ScoreValueComponent comp;
    comp.setScoreValue(250);
    EXPECT_EQ(comp.getScoreValue(), 250);
}

TEST(ScoreValueComponentTest, UpdateScoreValue) {
    ScoreValueComponent comp(50);
    EXPECT_EQ(comp.getScoreValue(), 50);
    
    comp.setScoreValue(150);
    EXPECT_EQ(comp.getScoreValue(), 150);
    
    comp.setScoreValue(300);
    EXPECT_EQ(comp.getScoreValue(), 300);
}

TEST(ScoreValueComponentTest, NegativeScoreValue) {
    ScoreValueComponent comp(-50);
    EXPECT_EQ(comp.getScoreValue(), -50);
    
    comp.setScoreValue(-100);
    EXPECT_EQ(comp.getScoreValue(), -100);
}

TEST(ScoreValueComponentTest, LargeScoreValue) {
    ScoreValueComponent comp(9999999);
    EXPECT_EQ(comp.getScoreValue(), 9999999);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
