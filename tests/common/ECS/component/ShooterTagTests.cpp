/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShooterTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ShooterTag.hpp"

using namespace ecs;

/* ShooterTag Tests */

TEST(ShooterTagTest, DefaultConstructor) {
    ShooterTag tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
