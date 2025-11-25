/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MobTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/tags/MobTag.hpp"

using namespace ecs;

/* MobTag Tests */

TEST(MobTagTest, DefaultConstructor) {
    MobTag tag;
    SUCCEED();
}

TEST(MobTagTest, InheritsFromAComponent) {
    MobTag tag;
    EXPECT_EQ(tag.getState(), ComponentState::Permanent);
}

TEST(MobTagTest, CanChangeState) {
    MobTag tag;
    tag.setState(ComponentState::Temporary);
    EXPECT_EQ(tag.getState(), ComponentState::Temporary);
    tag.setState(ComponentState::Processed);
    EXPECT_EQ(tag.getState(), ComponentState::Processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}