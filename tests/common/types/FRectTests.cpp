/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** FRect Tests
*/

#include <gtest/gtest.h>
#include "../../../common/types/FRect.hpp"

/* FRect Tests */

TEST(FRectTest, DefaultConstructor) {
    math::FRect rect;
    EXPECT_FLOAT_EQ(rect.getLeft(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getTop(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 0.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 0.0f);
}

TEST(FRectTest, ParameterizedConstructor) {
    math::FRect rect(1.5f, -2.3f, 10.0f, 20.5f);
    EXPECT_FLOAT_EQ(rect.getLeft(), 1.5f);
    EXPECT_FLOAT_EQ(rect.getTop(), -2.3f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 10.0f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 20.5f);
}

TEST(FRectTest, CopyConstructor) {
    math::FRect original(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect copy(original);
    EXPECT_FLOAT_EQ(copy.getLeft(), 1.0f);
    EXPECT_FLOAT_EQ(copy.getTop(), 2.0f);
    EXPECT_FLOAT_EQ(copy.getWidth(), 3.0f);
    EXPECT_FLOAT_EQ(copy.getHeight(), 4.0f);
}

TEST(FRectTest, GettersAndSetters) {
    math::FRect rect;
    rect.setLeft(3.14f);
    rect.setTop(-1.0f);
    rect.setWidth(5.5f);
    rect.setHeight(7.2f);
    EXPECT_FLOAT_EQ(rect.getLeft(), 3.14f);
    EXPECT_FLOAT_EQ(rect.getTop(), -1.0f);
    EXPECT_FLOAT_EQ(rect.getWidth(), 5.5f);
    EXPECT_FLOAT_EQ(rect.getHeight(), 7.2f);
}

TEST(FRectTest, AssignmentOperator) {
    math::FRect rect1(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect rect2;
    rect2 = rect1;
    EXPECT_FLOAT_EQ(rect2.getLeft(), 1.0f);
    EXPECT_FLOAT_EQ(rect2.getTop(), 2.0f);
    EXPECT_FLOAT_EQ(rect2.getWidth(), 3.0f);
    EXPECT_FLOAT_EQ(rect2.getHeight(), 4.0f);
}

TEST(FRectTest, EqualityOperator) {
    math::FRect rect1(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect rect2(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect rect3(1.1f, 2.0f, 3.0f, 4.0f);
    EXPECT_TRUE(rect1 == rect2);
    EXPECT_FALSE(rect1 == rect3);
}

TEST(FRectTest, InequalityOperator) {
    math::FRect rect1(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect rect2(1.0f, 2.0f, 3.0f, 4.0f);
    math::FRect rect3(1.1f, 2.0f, 3.0f, 4.0f);
    EXPECT_FALSE(rect1 != rect2);
    EXPECT_TRUE(rect1 != rect3);
}

TEST(FRectTest, ContainsPointInside) {
    math::FRect rect(0.0f, 0.0f, 10.0f, 10.0f);
    EXPECT_TRUE(rect.contains(5.0f, 5.0f));
    EXPECT_TRUE(rect.contains(0.0f, 0.0f));
    EXPECT_TRUE(rect.contains(9.9f, 9.9f));
}

TEST(FRectTest, ContainsPointOutside) {
    math::FRect rect(0.0f, 0.0f, 10.0f, 10.0f);
    EXPECT_FALSE(rect.contains(-1.0f, 5.0f));
    EXPECT_FALSE(rect.contains(5.0f, -1.0f));
    EXPECT_FALSE(rect.contains(10.0f, 5.0f));
    EXPECT_FALSE(rect.contains(5.0f, 10.0f));
    EXPECT_FALSE(rect.contains(11.0f, 11.0f));
}

TEST(FRectTest, ContainsWithNegativeDimensions) {
    math::FRect rect(10.0f, 10.0f, -5.0f, -5.0f); // width and height negative
    EXPECT_TRUE(rect.contains(6.0f, 6.0f)); // inside the actual bounds
    EXPECT_FALSE(rect.contains(11.0f, 11.0f)); // outside
}

TEST(FRectTest, IntersectsOverlapping) {
    math::FRect rect1(0.0f, 0.0f, 10.0f, 10.0f);
    math::FRect rect2(5.0f, 5.0f, 10.0f, 10.0f);
    EXPECT_TRUE(rect1.intersects(rect2));
}

TEST(FRectTest, IntersectsTouching) {
    math::FRect rect1(0.0f, 0.0f, 10.0f, 10.0f);
    math::FRect rect2(10.0f, 0.0f, 10.0f, 10.0f);
    EXPECT_FALSE(rect1.intersects(rect2)); // Touching at edge, but not overlapping
}

TEST(FRectTest, IntersectsSeparate) {
    math::FRect rect1(0.0f, 0.0f, 10.0f, 10.0f);
    math::FRect rect2(15.0f, 15.0f, 10.0f, 10.0f);
    EXPECT_FALSE(rect1.intersects(rect2));
}

TEST(FRectTest, IntersectsWithIntersectionRect) {
    math::FRect rect1(0.0f, 0.0f, 10.0f, 10.0f);
    math::FRect rect2(5.0f, 5.0f, 10.0f, 10.0f);
    math::FRect intersection;
    bool result = rect1.intersects(rect2, intersection);
    EXPECT_TRUE(result);
    EXPECT_FLOAT_EQ(intersection.getLeft(), 5.0f);
    EXPECT_FLOAT_EQ(intersection.getTop(), 5.0f);
    EXPECT_FLOAT_EQ(intersection.getWidth(), 5.0f);
    EXPECT_FLOAT_EQ(intersection.getHeight(), 5.0f);
}

TEST(FRectTest, IntersectsNoIntersection) {
    math::FRect rect1(0.0f, 0.0f, 10.0f, 10.0f);
    math::FRect rect2(15.0f, 15.0f, 10.0f, 10.0f);
    math::FRect intersection;
    bool result = rect1.intersects(rect2, intersection);
    EXPECT_FALSE(result);
    EXPECT_FLOAT_EQ(intersection.getLeft(), 0.0f);
    EXPECT_FLOAT_EQ(intersection.getTop(), 0.0f);
    EXPECT_FLOAT_EQ(intersection.getWidth(), 0.0f);
    EXPECT_FLOAT_EQ(intersection.getHeight(), 0.0f);
}

TEST(FRectTest, IntersectsWithNegativeDimensions) {
    math::FRect rect1(10.0f, 10.0f, -5.0f, -5.0f);
    math::FRect rect2(5.0f, 5.0f, 10.0f, 10.0f);
    math::FRect intersection;
    bool result = rect1.intersects(rect2, intersection);
    EXPECT_TRUE(result);
    // The intersection should be calculated correctly regardless of negative dimensions
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}