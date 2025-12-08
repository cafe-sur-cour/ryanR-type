/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIMovementPatternComponent
*/

#ifndef AIMOVEMENTPATTERNCOMPONENT_HPP
#define AIMOVEMENTPATTERNCOMPONENT_HPP

#include "../base/AComponent.hpp"

namespace ecs {

enum class AIMovementPattern {
    STRAIGHT_LINE,
    ZIGZAG,
    VERTICAL_MIRROR
};

class AIMovementPatternComponent : public AComponent {
    public:
        AIMovementPatternComponent(
            AIMovementPattern p = AIMovementPattern::STRAIGHT_LINE
        ) : pattern(p) {}
        ~AIMovementPatternComponent() = default;

        AIMovementPattern getPattern() const { return pattern; }
        float getZigzagAmplitude() const { return zigzagAmplitude; }
        float getZigzagFrequency() const { return zigzagFrequency; }
        float getDetectionRange() const { return detectionRange; }
        float getVerticalDeadzone() const { return verticalDeadzone; }
        float getTimer() const { return timer; }

        void setPattern(AIMovementPattern p) { pattern = p; }
        void setZigzagAmplitude(float amplitude) { zigzagAmplitude = amplitude; }
        void setZigzagFrequency(float frequency) { zigzagFrequency = frequency; }
        void setDetectionRange(float range) { detectionRange = range; }
        void setVerticalDeadzone(float deadzone) { verticalDeadzone = deadzone; }
        void setTimer(float t) { timer = t; }

    public:
        AIMovementPattern pattern;
        float zigzagAmplitude = 80.0f;
        float zigzagFrequency = 2.0f;
        float detectionRange = 800.0f;
        float verticalDeadzone = 10.0f;
        float timer = 0.0f;
};

} // namespace ecs

#endif /* !AIMOVEMENTPATTERNCOMPONENT_HPP */
