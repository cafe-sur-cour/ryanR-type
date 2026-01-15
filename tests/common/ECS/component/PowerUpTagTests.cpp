/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PowerUpTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/PowerUpTag.hpp"

using namespace ecs;

/* PowerUpTag Tests */

TEST(PowerUpTagTest, DefaultConstructor) {
    PowerUpTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
