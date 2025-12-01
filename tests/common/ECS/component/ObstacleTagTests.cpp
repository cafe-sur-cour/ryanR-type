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
    SUCCEED();
}

TEST(ObstacleTagTest, InheritsFromAComponent) {
    ObstacleTag tag;
    EXPECT_EQ(tag.getState(), ComponentState::Permanent);
}

TEST(ObstacleTagTest, CanChangeState) {
    ObstacleTag tag;
    tag.setState(ComponentState::Temporary);
    EXPECT_EQ(tag.getState(), ComponentState::Temporary);
    tag.setState(ComponentState::Processed);
    EXPECT_EQ(tag.getState(), ComponentState::Processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}