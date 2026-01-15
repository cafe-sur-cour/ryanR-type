/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ControllableTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ControllableTag.hpp"

using namespace ecs;

/* ControllableTag Tests */

TEST(ControllableTagTest, DefaultConstructor) {
    ControllableTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
