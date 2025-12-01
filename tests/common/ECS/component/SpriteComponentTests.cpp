/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** SpriteComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../client/components/rendering/SpriteComponent.hpp"

using namespace ecs;

/* SpriteComponent Tests */

TEST(SpriteComponentTest, DefaultConstructor) {
    SpriteComponent comp;
    EXPECT_EQ(comp.getTexturePath(), "");
    EXPECT_FALSE(comp.isValid());
}

TEST(SpriteComponentTest, ParameterizedConstructor) {
    std::string path = "assets/textures/player.png";
    SpriteComponent comp(path);
    EXPECT_EQ(comp.getTexturePath(), path);
    EXPECT_TRUE(comp.isValid());
}

TEST(SpriteComponentTest, TexturePathGetterAndSetter) {
    SpriteComponent comp;
    std::string newPath = "assets/textures/enemy.png";
    comp.setTexturePath(newPath);
    EXPECT_EQ(comp.getTexturePath(), newPath);
    EXPECT_TRUE(comp.isValid());
}

TEST(SpriteComponentTest, EmptyPathIsInvalid) {
    SpriteComponent comp;
    comp.setTexturePath("");
    EXPECT_EQ(comp.getTexturePath(), "");
    EXPECT_FALSE(comp.isValid());
}

TEST(SpriteComponentTest, NonEmptyPathIsValid) {
    SpriteComponent comp;
    comp.setTexturePath("some_texture.png");
    EXPECT_TRUE(comp.isValid());
}

TEST(SpriteComponentTest, PathWithSpaces) {
    std::string path = "assets/sprites/player ship.png";
    SpriteComponent comp(path);
    EXPECT_EQ(comp.getTexturePath(), path);
    EXPECT_TRUE(comp.isValid());
}

TEST(SpriteComponentTest, InheritsFromAComponent) {
    SpriteComponent comp;
    EXPECT_EQ(comp.getState(), ComponentState::Permanent);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}