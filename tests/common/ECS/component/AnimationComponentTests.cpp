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

TEST(AnimationComponentTest, DefaultConstructor) {
    AnimationComponent comp;

    EXPECT_EQ(comp.getCurrentState(), "");
    EXPECT_FALSE(comp.isPlaying());
    EXPECT_FALSE(comp.isValid());
}

TEST(AnimationComponentTest, AddState) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    EXPECT_TRUE(comp.isValid());
}

TEST(AnimationComponentTest, SetCurrentState) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    EXPECT_EQ(comp.getCurrentState(), "idle");
    EXPECT_TRUE(comp.isPlaying());
}

TEST(AnimationComponentTest, GetCurrentClip) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    const AnimationClip* currentClip = comp.getCurrentClip();
    ASSERT_NE(currentClip, nullptr);
    EXPECT_EQ(currentClip->texturePath, "texture.png");
    EXPECT_FLOAT_EQ(currentClip->frameWidth, 32.0f);
    EXPECT_FLOAT_EQ(currentClip->frameHeight, 32.0f);
    EXPECT_EQ(currentClip->frameCount, 3);
}

TEST(AnimationComponentTest, FrameRect) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    const math::FRect& rect = comp.getFrameRect();
    EXPECT_FLOAT_EQ(rect.getLeft(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getTop(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 32.0f);
}

TEST(AnimationComponentTest, SetFrameRect) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    math::FRect newRect(64.0f, 32.0f, 32.0f, 32.0f);
    comp.setFrameRect(newRect);

    const math::FRect& rect = comp.getFrameRect();
    EXPECT_FLOAT_EQ(rect.getLeft(), 64.0f);
    EXPECT_FLOAT_EQ(rect.getTop(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 32.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 32.0f);
}

TEST(AnimationComponentTest, CurrentFrame) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    EXPECT_EQ(comp.getCurrentFrame(), 0);

    comp.setCurrentFrame(2);
    EXPECT_EQ(comp.getCurrentFrame(), 2);
}

TEST(AnimationComponentTest, Timer) {
    AnimationComponent comp;
    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};

    comp.addState("idle", clip);
    comp.setCurrentState("idle");

    EXPECT_FLOAT_EQ(comp.getTimer(), 0.0f);

    comp.setTimer(1.5f);
    EXPECT_FLOAT_EQ(comp.getTimer(), 1.5f);
}

TEST(AnimationComponentTest, IsValid) {
    AnimationComponent comp;
    EXPECT_FALSE(comp.isValid());

    AnimationClip clip{"texture.png", 32.0f, 32.0f, 3, 0.0f, 0.0f, 0.1f, true};
    comp.addState("idle", clip);
    EXPECT_TRUE(comp.isValid());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}