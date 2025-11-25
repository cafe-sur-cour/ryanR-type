/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ProjectileTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/ProjectileTag.hpp"

using namespace ecs;

/* ProjectileTag Tests */

TEST(ProjectileTagTest, DefaultConstructor) {
    ProjectileTag tag;
    SUCCEED();
}

TEST(ProjectileTagTest, InheritsFromAComponent) {
    ProjectileTag tag;
    EXPECT_EQ(tag.getState(), ComponentState::Permanent);
}

TEST(ProjectileTagTest, CanChangeState) {
    ProjectileTag tag;
    tag.setState(ComponentState::Temporary);
    EXPECT_EQ(tag.getState(), ComponentState::Temporary);
    tag.setState(ComponentState::Processed);
    EXPECT_EQ(tag.getState(), ComponentState::Processed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}