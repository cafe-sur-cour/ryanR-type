/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ControllableTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/tags/ControllableTag.hpp"

using namespace ecs;

/* ControllableTag Tests */

TEST(ControllableTagTest, DefaultConstructor) {
    ControllableTag tag;
    SUCCEED();
}

TEST(ControllableTagTest, InheritsFromAComponent) {
    ControllableTag tag;
    EXPECT_EQ(tag.getState(), ComponentState::Permanent);
}

TEST(ControllableTagTest, CanChangeState) {
    ControllableTag tag;
    tag.setState(ComponentState::Temporary);
    EXPECT_EQ(tag.getState(), ComponentState::Temporary);
    tag.setState(ComponentState::Processed);
    EXPECT_EQ(tag.getState(), ComponentState::Processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
