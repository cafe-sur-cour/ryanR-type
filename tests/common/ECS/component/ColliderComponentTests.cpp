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

TEST(ColliderComponentTest, ScaleParameter) {
    math::Vector2f offset(5.0f, 10.0f);
    math::Vector2f size(20.0f, 30.0f);
    ColliderComponent comp(offset, size);
    math::Vector2f entityPos(50.0f, 60.0f);
    math::Vector2f scale(2.0f, 1.5f);

    math::FRect hitbox = comp.getHitbox(entityPos, scale);
    EXPECT_FLOAT_EQ(hitbox.getLeft(), 55.0f);   // 50 + 5
    EXPECT_FLOAT_EQ(hitbox.getTop(), 70.0f);    // 60 + 10
    EXPECT_FLOAT_EQ(hitbox.getWidth(), 40.0f);  // 20 * 2.0
    EXPECT_FLOAT_EQ(hitbox.getHeight(), 45.0f); // 30 * 1.5
}

TEST(ColliderComponentTest, DefaultScale) {
    math::Vector2f offset(1.0f, 2.0f);
    math::Vector2f size(10.0f, 15.0f);
    ColliderComponent comp(offset, size);
    math::Vector2f entityPos(0.0f, 0.0f);

    // Test with default scale (should be 1.0f, 1.0f)
    math::FRect hitboxDefault = comp.getHitbox(entityPos);
    math::FRect hitboxExplicit = comp.getHitbox(entityPos, math::Vector2f(1.0f, 1.0f));

    EXPECT_FLOAT_EQ(hitboxDefault.getLeft(), hitboxExplicit.getLeft());
    EXPECT_FLOAT_EQ(hitboxDefault.getTop(), hitboxExplicit.getTop());
    EXPECT_FLOAT_EQ(hitboxDefault.getWidth(), hitboxExplicit.getWidth());
    EXPECT_FLOAT_EQ(hitboxDefault.getHeight(), hitboxExplicit.getHeight());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}