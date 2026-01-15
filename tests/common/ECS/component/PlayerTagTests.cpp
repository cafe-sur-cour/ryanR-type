/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/PlayerTag.hpp"

using namespace ecs;

/* PlayerTag Tests */

TEST(PlayerTagTest, DefaultConstructor) {
    PlayerTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}