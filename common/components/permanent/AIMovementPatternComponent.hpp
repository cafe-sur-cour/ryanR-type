/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIMovementPatternComponent
*/

#ifndef AIMOVEMENTPATTERNCOMPONENT_HPP
#define AIMOVEMENTPATTERNCOMPONENT_HPP

#include "../base/AComponent.hpp"
#include "../../constants.hpp"

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
        float zigzagAmplitude = constants::DEFAULT_ZIGZAG_AMPLITUDE;
        float zigzagFrequency = constants::DEFAULT_ZIGZAG_FREQUENCY;
        float detectionRange = constants::DEFAULT_DETECTION_RANGE;
        float verticalDeadzone = constants::DEFAULT_VERTICAL_DEADZONE;
        float timer = constants::DEFAULT_TIMER;
};

} // namespace ecs

#endif /* !AIMOVEMENTPATTERNCOMPONENT_HPP */
