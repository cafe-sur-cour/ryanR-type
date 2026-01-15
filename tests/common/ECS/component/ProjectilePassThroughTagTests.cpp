/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ProjectilePassThroughTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ProjectilePassThroughTag.hpp"

using namespace ecs;

/* ProjectilePassThroughTag Tests */

TEST(ProjectilePassThroughTagTest, DefaultConstructor) {
    ProjectilePassThroughTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}