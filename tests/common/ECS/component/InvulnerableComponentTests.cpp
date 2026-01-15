/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** InvulnerableComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/InvulnerableComponent.hpp"

using namespace ecs;

/* InvulnerableComponent Tests */

TEST(InvulnerableComponentTest, DefaultConstructor) {
    InvulnerableComponent comp;
    EXPECT_FALSE(comp.isActive());
}

TEST(InvulnerableComponentTest, ParameterizedConstructorTrue) {
    InvulnerableComponent comp(true);
    EXPECT_TRUE(comp.isActive());
}

TEST(InvulnerableComponentTest, ParameterizedConstructorFalse) {
    InvulnerableComponent comp(false);
    EXPECT_FALSE(comp.isActive());
}

TEST(InvulnerableComponentTest, SetActiveTrue) {
    InvulnerableComponent comp(false);
    comp.setActive(true);
    EXPECT_TRUE(comp.isActive());
}

TEST(InvulnerableComponentTest, SetActiveFalse) {
    InvulnerableComponent comp(true);
    comp.setActive(false);
    EXPECT_FALSE(comp.isActive());
}

TEST(InvulnerableComponentTest, ToggleActiveState) {
    InvulnerableComponent comp(false);
    EXPECT_FALSE(comp.isActive());
    
    comp.setActive(true);
    EXPECT_TRUE(comp.isActive());
    
    comp.setActive(false);
    EXPECT_FALSE(comp.isActive());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
