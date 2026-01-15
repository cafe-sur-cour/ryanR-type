/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputIntentComponent Tests
*/

#include <gtest/gtest.h>
#include "../../../../common/components/temporary/InputIntentComponent.hpp"

using namespace ecs;

/* InputIntentComponent Tests */

TEST(InputIntentComponentTest, DefaultConstructor) {
    InputIntentComponent comp;
    math::Vector2f direction = comp.getDirection();
    EXPECT_FLOAT_EQ(direction.getX(), 0.0f);
    EXPECT_FLOAT_EQ(direction.getY(), 0.0f);
}

TEST(InputIntentComponentTest, ParameterizedConstructor) {
    math::Vector2f direction(1.0f, 2.0f);
    InputIntentComponent comp(direction);
    math::Vector2f retrieved = comp.getDirection();
    EXPECT_FLOAT_EQ(retrieved.getX(), 1.0f);
    EXPECT_FLOAT_EQ(retrieved.getY(), 2.0f);
}

TEST(InputIntentComponentTest, SetDirection) {
    InputIntentComponent comp;
    math::Vector2f newDirection(3.0f, 4.0f);
    comp.setDirection(newDirection);
    math::Vector2f retrieved = comp.getDirection();
    EXPECT_FLOAT_EQ(retrieved.getX(), 3.0f);
    EXPECT_FLOAT_EQ(retrieved.getY(), 4.0f);
}

TEST(InputIntentComponentTest, UpdateDirection) {
    InputIntentComponent comp(math::Vector2f(1.0f, 1.0f));
    math::Vector2f direction = comp.getDirection();
    EXPECT_FLOAT_EQ(direction.getX(), 1.0f);
    EXPECT_FLOAT_EQ(direction.getY(), 1.0f);
    
    comp.setDirection(math::Vector2f(-1.0f, -1.0f));
    direction = comp.getDirection();
    EXPECT_FLOAT_EQ(direction.getX(), -1.0f);
    EXPECT_FLOAT_EQ(direction.getY(), -1.0f);
}

TEST(InputIntentComponentTest, NegativeDirection) {
    InputIntentComponent comp(math::Vector2f(-5.0f, -10.0f));
    math::Vector2f direction = comp.getDirection();
    EXPECT_FLOAT_EQ(direction.getX(), -5.0f);
    EXPECT_FLOAT_EQ(direction.getY(), -10.0f);
}

TEST(InputIntentComponentTest, FloatPrecision) {
    InputIntentComponent comp(math::Vector2f(1.234f, 5.678f));
    math::Vector2f direction = comp.getDirection();
    EXPECT_FLOAT_EQ(direction.getX(), 1.234f);
    EXPECT_FLOAT_EQ(direction.getY(), 5.678f);
}

TEST(InputIntentComponentTest, Destructor) {
    InputIntentComponent* comp = new InputIntentComponent(math::Vector2f(1.0f, 2.0f));
    delete comp;
    SUCCEED();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
