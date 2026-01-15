/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** MobTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/MobTag.hpp"

using namespace ecs;

/* MobTag Tests */

TEST(MobTagTest, DefaultConstructor) {
    MobTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}