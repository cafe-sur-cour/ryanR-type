/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** TransformComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/ECS/component/TransformComponent.hpp"

using namespace ecs;

/* TransformComponent Tests */

TEST(TransformComponentTest, DefaultConstructor) {
    TransformComponent comp;
    Vector2f expectedPos(0.0f, 0.0f);
    Vector2f expectedScale(1.0f, 1.0f);
    EXPECT_EQ(comp.getPosition().getX(), expectedPos.getX());
    EXPECT_EQ(comp.getPosition().getY(), expectedPos.getY());
    EXPECT_FLOAT_EQ(comp.getRotation(), 0.0f);
    EXPECT_EQ(comp.getScale().getX(), expectedScale.getX());
    EXPECT_EQ(comp.getScale().getY(), expectedScale.getY());
}

TEST(TransformComponentTest, ParameterizedConstructor) {
    Vector2f pos(1.0f, 2.0f);
    float rot = 45.0f;
    Vector2f scale(2.0f, 3.0f);
    TransformComponent comp(pos, rot, scale);
    EXPECT_EQ(comp.getPosition().getX(), 1.0f);
    EXPECT_EQ(comp.getPosition().getY(), 2.0f);
    EXPECT_FLOAT_EQ(comp.getRotation(), 45.0f);
    EXPECT_EQ(comp.getScale().getX(), 2.0f);
    EXPECT_EQ(comp.getScale().getY(), 3.0f);
}

TEST(TransformComponentTest, PositionGettersAndSetters) {
    TransformComponent comp;
    Vector2f newPos(10.0f, 20.0f);
    comp.setPosition(newPos);
    EXPECT_EQ(comp.getPosition().getX(), 10.0f);
    EXPECT_EQ(comp.getPosition().getY(), 20.0f);
}

TEST(TransformComponentTest, RotationGettersAndSetters) {
    TransformComponent comp;
    comp.setRotation(90.0f);
    EXPECT_FLOAT_EQ(comp.getRotation(), 90.0f);
}

TEST(TransformComponentTest, ScaleGettersAndSetters) {
    TransformComponent comp;
    Vector2f newScale(0.5f, 1.5f);
    comp.setScale(newScale);
    EXPECT_EQ(comp.getScale().getX(), 0.5f);
    EXPECT_EQ(comp.getScale().getY(), 1.5f);
}

TEST(TransformComponentTest, InheritsFromAComponent) {
    TransformComponent comp;
    EXPECT_EQ(comp.getState(), ComponentState::Permanent);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}