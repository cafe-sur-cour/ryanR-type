/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ObstacleTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ObstacleTag.hpp"

using namespace ecs;

/* ObstacleTag Tests */

TEST(ObstacleTagTest, DefaultConstructor) {
    ObstacleTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}