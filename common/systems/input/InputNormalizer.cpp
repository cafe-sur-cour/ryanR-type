/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputNormalizer
*/

#include "InputNormalizer.hpp"
#include "../../constants.hpp"
#include <cmath>

namespace ecs {

math::Vector2f InputNormalizer::normalizeDirection(const math::Vector2f &direction) {
    math::Vector2f normalized = direction;
    float magnitude = std::sqrt(normalized.getX() * normalized.getX() +
                               normalized.getY() * normalized.getY());
    if (magnitude > 1.0f) {
        normalized.setX(normalized.getX() / magnitude);
        normalized.setY(normalized.getY() / magnitude);
    }
    return normalized;
}

math::Vector2f InputNormalizer::normalizeAnalogInput(float rawX, float rawY) {
    const float deadzone = constants::GAMEPAD_DEADZONE * 100.0f;
    const float maxValue = constants::AXIS_MAX_VALUE;
    const float range = maxValue - deadzone;

    float normalizedX = rawX;
    float normalizedY = rawY;

    if (std::abs(normalizedX) < deadzone) {
        normalizedX = 0.0f;
    } else {
        float sign = normalizedX > 0 ? 1.0f : -1.0f;
        normalizedX = (std::abs(normalizedX) - deadzone) / range * sign;
    }
    if (std::abs(normalizedY) < deadzone) {
        normalizedY = 0.0f;
    } else {
        float sign = normalizedY > 0 ? 1.0f : -1.0f;
        normalizedY = (std::abs(normalizedY) - deadzone) / range * sign;
    }
    math::Vector2f result(normalizedX, normalizedY);
    float magnitude = std::sqrt(result.getX() * result.getX() +
                               result.getY() * result.getY());
    if (magnitude > 1.0f) {
        result.setX(result.getX() / magnitude);
        result.setY(result.getY() / magnitude);
    }

    return result;
}

} // namespace ecs