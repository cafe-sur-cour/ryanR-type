/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** AnimationComponent Tests
*/

#include <gtest/gtest.h>
#include <thread>
#include "../../../../client/components/rendering/AnimationComponent.hpp"

using namespace ecs;

/* AnimationComponent Tests */

TEST(AnimationComponentTest, ConstructorWithParameters) {
    std::string texturePath = "assets/sprites/player.png";
    float frameWidth = 64.0f;
    float frameHeight = 64.0f;
    int frameCount = 4;
    float startWidth = 0.0f;
    float startHeight = 0.0f;
    float speed = 0.2f;

    AnimationComponent comp(texturePath, frameWidth, frameHeight, frameCount, startWidth, startHeight, speed);

    EXPECT_EQ(comp.getTexturePath(), texturePath);
    EXPECT_FLOAT_EQ(comp.getFrameWidth(), frameWidth);
    EXPECT_FLOAT_EQ(comp.getFrameHeight(), frameHeight);
    EXPECT_EQ(comp.getFrameCount(), frameCount);
    EXPECT_FLOAT_EQ(comp.getAnimationSpeed(), speed);
    EXPECT_EQ(comp.getCurrentFrame(), 0);
    EXPECT_TRUE(comp.isValid());
    EXPECT_TRUE(comp.getChrono().isRunning()); // Should start automatically
    EXPECT_FLOAT_EQ(comp.getStartWidth(), startWidth);
    EXPECT_FLOAT_EQ(comp.getStartHeight(), startHeight);
}

TEST(AnimationComponentTest, DefaultFrameRect) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f);

    const math::FRect& rect = comp.getFrameRect();
    EXPECT_FLOAT_EQ(rect.getLeft(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getTop(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 32.0f);
}

TEST(AnimationComponentTest, SetFrameRect) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f);
    math::FRect newRect(64.0f, 32.0f, 32.0f, 32.0f);

    comp.setFrameRect(newRect);
    const math::FRect& rect = comp.getFrameRect();

    EXPECT_FLOAT_EQ(rect.getLeft(), 64.0f);
    EXPECT_FLOAT_EQ(rect.getTop(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 32.0f);
}

TEST(AnimationComponentTest, CurrentFrameGetterAndSetter) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f);

    EXPECT_EQ(comp.getCurrentFrame(), 0);

    comp.setCurrentFrame(2);
    EXPECT_EQ(comp.getCurrentFrame(), 2);

    comp.setCurrentFrame(5); // Should work even if > frameCount
    EXPECT_EQ(comp.getCurrentFrame(), 5);
}

TEST(AnimationComponentTest, AnimationSpeedGetterAndSetter) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f);

    EXPECT_FLOAT_EQ(comp.getAnimationSpeed(), 0.1f);

    comp.setAnimationSpeed(0.5f);
    EXPECT_FLOAT_EQ(comp.getAnimationSpeed(), 0.5f);
}

TEST(AnimationComponentTest, ChronoAccess) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f);

    // Chrono should be running from constructor
    EXPECT_TRUE(comp.getChrono().isRunning());

    // Can access const version
    const AnimationComponent& constComp = comp;
    EXPECT_TRUE(constComp.getChrono().isRunning());
}

TEST(AnimationComponentTest, IsValid) {
    // Valid component
    AnimationComponent validComp("texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f);
    EXPECT_TRUE(validComp.isValid());

    // Invalid: zero frame count
    AnimationComponent invalidCount("texture.png", 32.0f, 32.0f, 0, 0.0f, 0.0f);
    EXPECT_FALSE(invalidCount.isValid());

    // Invalid: zero width
    AnimationComponent invalidWidth("texture.png", 0.0f, 32.0f, 3, 0.0f, 0.0f);
    EXPECT_FALSE(invalidWidth.isValid());

    // Invalid: zero height
    AnimationComponent invalidHeight("texture.png", 32.0f, 0.0f, 3, 0.0f, 0.0f);
    EXPECT_FALSE(invalidHeight.isValid());
}

TEST(AnimationComponentTest, TexturePathGetter) {
    std::string path = "assets/animations/walk.png";
    AnimationComponent comp(path, 32.0f, 32.0f, 4, 0.0f, 0.0f);

    EXPECT_EQ(comp.getTexturePath(), path);
}

TEST(AnimationComponentTest, StartWidthAndHeightGetterAndSetter) {
    AnimationComponent comp("texture.png", 32.0f, 32.0f, 3, 10.0f, 20.0f);

    EXPECT_FLOAT_EQ(comp.getStartWidth(), 10.0f);
    EXPECT_FLOAT_EQ(comp.getStartHeight(), 20.0f);

    comp.setStartWidth(15.0f);
    comp.setStartHeight(25.0f);

    EXPECT_FLOAT_EQ(comp.getStartWidth(), 15.0f);
    EXPECT_FLOAT_EQ(comp.getStartHeight(), 25.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}