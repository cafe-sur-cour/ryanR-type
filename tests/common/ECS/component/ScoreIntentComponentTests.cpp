/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/ScoreIntentComponent.hpp"

/* ScoreIntentComponent Tests */

TEST(ScoreIntentComponentTest, DefaultConstructor) {
    ScoreIntentComponent comp;
    EXPECT_EQ(comp.getScore(), 0);
}

TEST(ScoreIntentComponentTest, ParameterizedConstructor) {
    ScoreIntentComponent comp(25);
    EXPECT_EQ(comp.getScore(), 25);
}

TEST(ScoreIntentComponentTest, GetScore) {
    ScoreIntentComponent comp(75);
    EXPECT_EQ(comp.getScore(), 75);
}

TEST(ScoreIntentComponentTest, SetScore) {
    ScoreIntentComponent comp;
    comp.setScore(50);
    EXPECT_EQ(comp.getScore(), 50);
}

TEST(ScoreIntentComponentTest, NegativeScore) {
    ScoreIntentComponent comp(-10);
    EXPECT_EQ(comp.getScore(), -10);
}

TEST(ScoreIntentComponentTest, ZeroScore) {
    ScoreIntentComponent comp(0);
    EXPECT_EQ(comp.getScore(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}