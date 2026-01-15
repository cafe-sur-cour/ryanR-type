/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientEffectTag Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/tags/ClientEffectTag.hpp"

using namespace ecs;

/* ClientEffectTag Tests */

TEST(ClientEffectTagTest, DefaultConstructor) {
    ClientEffectTag tag;
    (void)tag;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
