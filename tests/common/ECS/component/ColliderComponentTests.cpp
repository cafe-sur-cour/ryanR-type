/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ColliderComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/permanent/ColliderComponent.hpp"

using namespace ecs;

/* ColliderComponent Tests */

TEST(ColliderComponentTest, DefaultConstructor) {
    ColliderComponent comp;
    math::FRect expectedHitbox(0.0f, 0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getLeft(), expectedHitbox.getLeft());
    EXPECT_FLOAT_EQ(comp.getHitbox().getTop(), expectedHitbox.getTop());
    EXPECT_FLOAT_EQ(comp.getHitbox().getWidth(), expectedHitbox.getWidth());
    EXPECT_FLOAT_EQ(comp.getHitbox().getHeight(), expectedHitbox.getHeight());
}

TEST(ColliderComponentTest, ParameterizedConstructor) {
    math::FRect hitbox(1.0f, 2.0f, 10.0f, 20.0f);
    ColliderComponent comp(hitbox);
    EXPECT_FLOAT_EQ(comp.getHitbox().getLeft(), 1.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getTop(), 2.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getWidth(), 10.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getHeight(), 20.0f);
}

TEST(ColliderComponentTest, HitboxGettersAndSetters) {
    ColliderComponent comp;
    math::FRect newHitbox(5.0f, 5.0f, 15.0f, 25.0f);
    comp.setHitbox(newHitbox);
    EXPECT_FLOAT_EQ(comp.getHitbox().getLeft(), 5.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getTop(), 5.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getWidth(), 15.0f);
    EXPECT_FLOAT_EQ(comp.getHitbox().getHeight(), 25.0f);
}

TEST(ColliderComponentTest, InheritsFromAComponent) {
    ColliderComponent comp;
    EXPECT_EQ(comp.getState(), ComponentState::Permanent);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}