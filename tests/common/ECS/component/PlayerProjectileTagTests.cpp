/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** PlayerProjectileTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/PlayerProjectileTag.hpp"

using namespace ecs;

/* PlayerProjectileTag Tests */

TEST(PlayerProjectileTagTest, DefaultConstructor) {
    PlayerProjectileTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}