/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpeedComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/SpeedComponent.hpp"

using namespace ecs;

/* SpeedComponent Tests */

TEST(SpeedComponentTest, DefaultConstructor) {
    SpeedComponent comp;
    EXPECT_EQ(comp.getSpeed(), constants::BASE_SPEED);
}

TEST(SpeedComponentTest, ParameterizedConstructor) {
    SpeedComponent comp(150.0f);
    EXPECT_EQ(comp.getSpeed(), 150.0f);
}

TEST(SpeedComponentTest, GettersAndSetters) {
    SpeedComponent comp;
    comp.setSpeed(200.0f);
    EXPECT_EQ(comp.getSpeed(), 200.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}