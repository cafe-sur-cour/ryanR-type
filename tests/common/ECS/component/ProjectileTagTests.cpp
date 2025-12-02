/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ProjectileTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ProjectileTag.hpp"

using namespace ecs;

/* ProjectileTag Tests */

TEST(ProjectileTagTest, DefaultConstructor) {
    ProjectileTag tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
