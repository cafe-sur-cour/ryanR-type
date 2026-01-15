/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ProjectilePrefabComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ProjectilePrefabComponent.hpp"

using namespace ecs;

/* ProjectilePrefabComponent Tests */

TEST(ProjectilePrefabComponentTest, DefaultConstructor) {
    ProjectilePrefabComponent comp;
    EXPECT_EQ(comp.getPrefabName(), "");
}

TEST(ProjectilePrefabComponentTest, ParameterizedConstructor) {
    ProjectilePrefabComponent comp("bullet");
    EXPECT_EQ(comp.getPrefabName(), "bullet");
}

TEST(ProjectilePrefabComponentTest, SetPrefabName) {
    ProjectilePrefabComponent comp;
    comp.setPrefabName("laser");
    EXPECT_EQ(comp.getPrefabName(), "laser");
}

TEST(ProjectilePrefabComponentTest, UpdatePrefabName) {
    ProjectilePrefabComponent comp("missile");
    EXPECT_EQ(comp.getPrefabName(), "missile");
    
    comp.setPrefabName("rocket");
    EXPECT_EQ(comp.getPrefabName(), "rocket");
}

TEST(ProjectilePrefabComponentTest, EmptyString) {
    ProjectilePrefabComponent comp("");
    EXPECT_EQ(comp.getPrefabName(), "");
    
    comp.setPrefabName("test");
    EXPECT_EQ(comp.getPrefabName(), "test");
    
    comp.setPrefabName("");
    EXPECT_EQ(comp.getPrefabName(), "");
}

TEST(ProjectilePrefabComponentTest, LongString) {
    std::string longName = "very_long_projectile_prefab_name_with_many_characters";
    ProjectilePrefabComponent comp(longName);
    EXPECT_EQ(comp.getPrefabName(), longName);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
