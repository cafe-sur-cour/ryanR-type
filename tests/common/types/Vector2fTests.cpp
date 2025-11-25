/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Vector2f Tests
*/

#include <gtest/gtest.h>
#include "../../../common/types/Vector2f.hpp"

/* Vector2f Tests */

TEST(Vector2fTest, DefaultConstructor) {
    math::Vector2f vec;
    EXPECT_FLOAT_EQ(vec.getX(), 0.0f);
    EXPECT_FLOAT_EQ(vec.getY(), 0.0f);
}

TEST(Vector2fTest, ParameterizedConstructor) {
    math::Vector2f vec(1.5f, -2.3f);
    EXPECT_FLOAT_EQ(vec.getX(), 1.5f);
    EXPECT_FLOAT_EQ(vec.getY(), -2.3f);
}

TEST(Vector2fTest, GettersAndSetters) {
    math::Vector2f vec;
    vec.setX(3.14f);
    vec.setY(-1.0f);
    EXPECT_FLOAT_EQ(vec.getX(), 3.14f);
    EXPECT_FLOAT_EQ(vec.getY(), -1.0f);
}

TEST(Vector2fTest, AssignmentOperator) {
    math::Vector2f vec1(1.0f, 2.0f);
    math::Vector2f vec2;
    vec2 = vec1;
    EXPECT_FLOAT_EQ(vec2.getX(), 1.0f);
    EXPECT_FLOAT_EQ(vec2.getY(), 2.0f);
}

TEST(Vector2fTest, AdditionAssignmentOperator) {
    math::Vector2f vec1(1.0f, 2.0f);
    math::Vector2f vec2(0.5f, -1.0f);
    vec1 += vec2;
    EXPECT_FLOAT_EQ(vec1.getX(), 1.5f);
    EXPECT_FLOAT_EQ(vec1.getY(), 1.0f);
}

TEST(Vector2fTest, SubtractionAssignmentOperator) {
    math::Vector2f vec1(1.0f, 2.0f);
    math::Vector2f vec2(0.5f, 1.0f);
    vec1 -= vec2;
    EXPECT_FLOAT_EQ(vec1.getX(), 0.5f);
    EXPECT_FLOAT_EQ(vec1.getY(), 1.0f);
}

TEST(Vector2fTest, MultiplicationAssignmentOperator) {
    math::Vector2f vec(1.0f, 2.0f);
    vec *= 3.0f;
    EXPECT_FLOAT_EQ(vec.getX(), 3.0f);
    EXPECT_FLOAT_EQ(vec.getY(), 6.0f);
}

TEST(Vector2fTest, DivisionAssignmentOperator) {
    math::Vector2f vec(6.0f, 8.0f);
    vec /= 2.0f;
    EXPECT_FLOAT_EQ(vec.getX(), 3.0f);
    EXPECT_FLOAT_EQ(vec.getY(), 4.0f);
}

TEST(Vector2fTest, MultiplicationOperator) {
    math::Vector2f vec(1.0f, 2.0f);
    math::Vector2f result = vec * 3.0f;
    EXPECT_FLOAT_EQ(result.getX(), 3.0f);
    EXPECT_FLOAT_EQ(result.getY(), 6.0f);
}

TEST(Vector2fTest, AdditionOperator) {
    math::Vector2f vec1(1.0f, 2.0f);
    math::Vector2f vec2(0.5f, -1.0f);
    math::Vector2f result = vec1 + vec2;
    EXPECT_FLOAT_EQ(result.getX(), 1.5f);
    EXPECT_FLOAT_EQ(result.getY(), 1.0f);
}

TEST(Vector2fTest, SubtractionOperator) {
    math::Vector2f vec1(1.0f, 2.0f);
    math::Vector2f vec2(0.5f, 1.0f);
    math::Vector2f result = vec1 - vec2;
    EXPECT_FLOAT_EQ(result.getX(), 0.5f);
    EXPECT_FLOAT_EQ(result.getY(), 1.0f);
}

TEST(Vector2fTest, GetVector) {
    math::Vector2f vec(1.0f, 2.0f);
    math::Vector2f copy = vec.getVector();
    EXPECT_FLOAT_EQ(copy.getX(), 1.0f);
    EXPECT_FLOAT_EQ(copy.getY(), 2.0f);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}