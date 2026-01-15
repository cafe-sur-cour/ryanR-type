/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputNormalizer Tests
*/

#include <gtest/gtest.h>
#include <cmath>
#include "../../../../common/systems/input/InputNormalizer.hpp"
#include "../../../../common/constants.hpp"

using namespace ecs;

/* InputNormalizer Tests */

class InputNormalizerTest : public ::testing::Test {
protected:
    const float epsilon = 0.0001f;
};

TEST_F(InputNormalizerTest, NormalizeDirection_ZeroVector_ReturnsZero) {
    math::Vector2f input(0.0f, 0.0f);
    auto result = InputNormalizer::normalizeDirection(input);

    EXPECT_FLOAT_EQ(result.getX(), 0.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeDirection_UnitVector_Unchanged) {
    math::Vector2f input(1.0f, 0.0f);
    auto result = InputNormalizer::normalizeDirection(input);

    EXPECT_FLOAT_EQ(result.getX(), 1.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeDirection_DiagonalUnit_Unchanged) {
    float diagonal = std::sqrt(2.0f) / 2.0f;
    math::Vector2f input(diagonal, diagonal);
    auto result = InputNormalizer::normalizeDirection(input);

    // Magnitude should be approximately 1
    float magnitude = std::sqrt(result.getX() * result.getX() + result.getY() * result.getY());
    EXPECT_NEAR(magnitude, 1.0f, epsilon);
}

TEST_F(InputNormalizerTest, NormalizeDirection_LargeVector_Normalized) {
    math::Vector2f input(3.0f, 4.0f); // Magnitude = 5
    auto result = InputNormalizer::normalizeDirection(input);

    EXPECT_NEAR(result.getX(), 0.6f, epsilon);
    EXPECT_NEAR(result.getY(), 0.8f, epsilon);

    // Check magnitude is 1
    float magnitude = std::sqrt(result.getX() * result.getX() + result.getY() * result.getY());
    EXPECT_NEAR(magnitude, 1.0f, epsilon);
}

TEST_F(InputNormalizerTest, NormalizeDirection_VectorBelowOne_Unchanged) {
    math::Vector2f input(0.5f, 0.3f);
    auto result = InputNormalizer::normalizeDirection(input);

    EXPECT_FLOAT_EQ(result.getX(), 0.5f);
    EXPECT_FLOAT_EQ(result.getY(), 0.3f);
}

TEST_F(InputNormalizerTest, NormalizeDirection_NegativeValues_HandledCorrectly) {
    math::Vector2f input(-3.0f, -4.0f);
    auto result = InputNormalizer::normalizeDirection(input);

    EXPECT_NEAR(result.getX(), -0.6f, epsilon);
    EXPECT_NEAR(result.getY(), -0.8f, epsilon);

    float magnitude = std::sqrt(result.getX() * result.getX() + result.getY() * result.getY());
    EXPECT_NEAR(magnitude, 1.0f, epsilon);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_ZeroInput_ReturnsZero) {
    auto result = InputNormalizer::normalizeAnalogInput(0.0f, 0.0f);

    EXPECT_FLOAT_EQ(result.getX(), 0.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_BelowDeadzone_ReturnsZero) {
    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    const float belowDeadzone = deadzone * 0.5f;

    auto result = InputNormalizer::normalizeAnalogInput(belowDeadzone, belowDeadzone);

    EXPECT_FLOAT_EQ(result.getX(), 0.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_AtMaxValue_ReturnsOne) {
    const float maxValue = constants::AXIS_MAX_VALUE;

    auto result = InputNormalizer::normalizeAnalogInput(maxValue, 0.0f);

    EXPECT_GT(result.getX(), 0.9f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_AboveDeadzone_Normalized) {
    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    const float aboveDeadzone = deadzone * 2.0f;

    auto result = InputNormalizer::normalizeAnalogInput(aboveDeadzone, 0.0f);

    EXPECT_GT(result.getX(), 0.0f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_NegativeValues_HandledCorrectly) {
    const float maxValue = constants::AXIS_MAX_VALUE;

    auto result = InputNormalizer::normalizeAnalogInput(-maxValue, -maxValue);

    EXPECT_LT(result.getX(), 0.0f);
    EXPECT_LT(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_MixedDeadzone_OnlyActiveAxisNormalized) {
    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    const float maxValue = constants::AXIS_MAX_VALUE;
    const float belowDeadzone = deadzone * 0.5f;

    auto result = InputNormalizer::normalizeAnalogInput(maxValue, belowDeadzone);

    EXPECT_GT(result.getX(), 0.9f);
    EXPECT_FLOAT_EQ(result.getY(), 0.0f);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_LargeValues_ClampedToUnitCircle) {
    const float maxValue = constants::AXIS_MAX_VALUE;

    auto result = InputNormalizer::normalizeAnalogInput(maxValue, maxValue);

    // Should be clamped to unit circle
    float magnitude = std::sqrt(result.getX() * result.getX() + result.getY() * result.getY());
    EXPECT_NEAR(magnitude, 1.0f, epsilon);
}

TEST_F(InputNormalizerTest, NormalizeAnalogInput_AsymmetricInput_HandledCorrectly) {
    const float maxValue = constants::AXIS_MAX_VALUE;

    auto result = InputNormalizer::normalizeAnalogInput(maxValue, maxValue * 0.5f);

    EXPECT_GT(result.getX(), 0.0f);
    EXPECT_GT(result.getY(), 0.0f);

    // Magnitude should not exceed 1
    float magnitude = std::sqrt(result.getX() * result.getX() + result.getY() * result.getY());
    EXPECT_LE(magnitude, 1.0f + epsilon);
}
