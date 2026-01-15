/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** EnnemyProjectileTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/EnnemyProjectileTag.hpp"

using namespace ecs;

/* EnnemyProjectileTag Tests */

TEST(EnnemyProjectileTagTest, DefaultConstructor) {
    EnnemyProjectileTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}