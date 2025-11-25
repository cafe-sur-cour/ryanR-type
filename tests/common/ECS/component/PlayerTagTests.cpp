/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/tags/PlayerTag.hpp"

using namespace ecs;

/* PlayerTag Tests */

TEST(PlayerTagTest, DefaultConstructor) {
    PlayerTag tag;
    SUCCEED();
}

TEST(PlayerTagTest, InheritsFromAComponent) {
    PlayerTag tag;
    EXPECT_EQ(tag.getState(), ComponentState::Permanent);
}

TEST(PlayerTagTest, CanChangeState) {
    PlayerTag tag;
    tag.setState(ComponentState::Temporary);
    EXPECT_EQ(tag.getState(), ComponentState::Temporary);
    tag.setState(ComponentState::Processed);
    EXPECT_EQ(tag.getState(), ComponentState::Processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}