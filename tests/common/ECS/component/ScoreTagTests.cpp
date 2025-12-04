/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ScoreTag.hpp"

using namespace ecs;

/* ScoreTag Tests */

TEST(ScoreTagTest, DefaultConstructor) {
    ScoreTag tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}