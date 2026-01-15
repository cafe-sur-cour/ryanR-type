/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OwnerComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/OwnerComponent.hpp"

using namespace ecs;

/* OwnerComponent Tests */

TEST(OwnerComponentTest, DefaultConstructor) {
    OwnerComponent comp;
    EXPECT_EQ(comp.getOwner(), 0);
}

TEST(OwnerComponentTest, ParameterizedConstructor) {
    OwnerComponent comp(42);
    EXPECT_EQ(comp.getOwner(), 42);
}

TEST(OwnerComponentTest, SetOwner) {
    OwnerComponent comp;
    comp.setOwner(123);
    EXPECT_EQ(comp.getOwner(), 123);
}

TEST(OwnerComponentTest, UpdateOwner) {
    OwnerComponent comp(10);
    EXPECT_EQ(comp.getOwner(), 10);
    
    comp.setOwner(20);
    EXPECT_EQ(comp.getOwner(), 20);
    
    comp.setOwner(30);
    EXPECT_EQ(comp.getOwner(), 30);
}

TEST(OwnerComponentTest, LargeEntityId) {
    OwnerComponent comp(999999);
    EXPECT_EQ(comp.getOwner(), 999999);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
