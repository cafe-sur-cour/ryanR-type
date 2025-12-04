/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ScoreComponent.hpp"

using namespace ecs;

/* ScoreComponent Tests */

TEST(ScoreComponentTest, DefaultConstructor) {
    ScoreComponent comp;
    EXPECT_EQ(comp.getScore(), 0);
}

TEST(ScoreComponentTest, ParameterizedConstructor) {
    ScoreComponent comp(50);
    EXPECT_EQ(comp.getScore(), 50);
}

TEST(ScoreComponentTest, GetScore) {
    ScoreComponent comp(75);
    EXPECT_EQ(comp.getScore(), 75);
}

TEST(ScoreComponentTest, SetScore) {
    ScoreComponent comp;
    comp.setScore(25);
    EXPECT_EQ(comp.getScore(), 25);
}

TEST(ScoreComponentTest, AddScore) {
    ScoreComponent comp(10);
    comp.addScore(15);
    EXPECT_EQ(comp.getScore(), 25);
}

TEST(ScoreComponentTest, AddNegativeScore) {
    ScoreComponent comp(20);
    comp.addScore(-5);
    EXPECT_EQ(comp.getScore(), 15);
}

TEST(ScoreComponentTest, NegativeScore) {
    ScoreComponent comp(-10);
    EXPECT_EQ(comp.getScore(), -10);
}

TEST(ScoreComponentTest, ZeroScore) {
    ScoreComponent comp(0);
    EXPECT_EQ(comp.getScore(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}