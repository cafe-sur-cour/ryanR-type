/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** ColliderComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/permanent/ColliderComponent.hpp"

using namespace ecs;

/* ColliderComponent Tests */

TEST(ColliderComponentTest, DefaultConstructor) {
    ColliderComponent comp;
    math::Vector2f entityPos(0.0f, 0.0f);
    math::FRect hitbox = comp.getHitbox(entityPos);
    EXPECT_FLOAT_EQ(hitbox.getLeft(), 0.0f);
    EXPECT_FLOAT_EQ(hitbox.getTop(), 0.0f);
    EXPECT_FLOAT_EQ(hitbox.getWidth(), 0.0f);
    EXPECT_FLOAT_EQ(hitbox.getHeight(), 0.0f);
    EXPECT_EQ(comp.getType(), CollisionType::Solid);
}

TEST(ColliderComponentTest, ParameterizedConstructor) {
    math::Vector2f offset(1.0f, 2.0f);
    math::Vector2f size(10.0f, 20.0f);
    ColliderComponent comp(offset, size, CollisionType::Bounce);
    math::Vector2f entityPos(0.0f, 0.0f);
    math::FRect hitbox = comp.getHitbox(entityPos);
    EXPECT_FLOAT_EQ(hitbox.getLeft(), 1.0f);
    EXPECT_FLOAT_EQ(hitbox.getTop(), 2.0f);
    EXPECT_FLOAT_EQ(hitbox.getWidth(), 10.0f);
    EXPECT_FLOAT_EQ(hitbox.getHeight(), 20.0f);
    EXPECT_EQ(comp.getType(), CollisionType::Bounce);
}

TEST(ColliderComponentTest, OffsetAndSizeGettersAndSetters) {
    ColliderComponent comp;
    math::Vector2f newOffset(5.0f, 5.0f);
    math::Vector2f newSize(15.0f, 25.0f);
    comp.setOffset(newOffset);
    comp.setSize(newSize);
    math::Vector2f entityPos(0.0f, 0.0f);
    math::FRect hitbox = comp.getHitbox(entityPos);
    EXPECT_FLOAT_EQ(hitbox.getLeft(), 5.0f);
    EXPECT_FLOAT_EQ(hitbox.getTop(), 5.0f);
    EXPECT_FLOAT_EQ(hitbox.getWidth(), 15.0f);
    EXPECT_FLOAT_EQ(hitbox.getHeight(), 25.0f);
}

TEST(ColliderComponentTest, TypeGettersAndSetters) {
    ColliderComponent comp;
    EXPECT_EQ(comp.getType(), CollisionType::Solid);  // Default
    comp.setType(CollisionType::Trigger);
    EXPECT_EQ(comp.getType(), CollisionType::Trigger);
}

TEST(ColliderComponentTest, RelativePosition) {
    math::Vector2f offset(10.0f, 20.0f);
    math::Vector2f size(30.0f, 40.0f);
    ColliderComponent comp(offset, size);
    math::Vector2f entityPos(100.0f, 200.0f);
    math::FRect hitbox = comp.getHitbox(entityPos);
    EXPECT_FLOAT_EQ(hitbox.getLeft(), 110.0f);  // 100 + 10
    EXPECT_FLOAT_EQ(hitbox.getTop(), 220.0f);   // 200 + 20
    EXPECT_FLOAT_EQ(hitbox.getWidth(), 30.0f);
    EXPECT_FLOAT_EQ(hitbox.getHeight(), 40.0f);
}

TEST(ColliderComponentTest, InheritsFromAComponent) {
    ColliderComponent comp;
    EXPECT_EQ(comp.getState(), ComponentState::Permanent);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}