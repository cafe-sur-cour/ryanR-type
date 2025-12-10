/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputNormalizer
*/

#ifndef INPUTNORMALIZER_HPP_
#define INPUTNORMALIZER_HPP_

#include "../../types/Vector2f.hpp"

namespace ecs {

class InputNormalizer {
    public:
        static math::Vector2f normalizeDirection(const math::Vector2f &direction);
        static math::Vector2f normalizeAnalogInput(float rawX, float rawY);
};

} // namespace ecs

#endif /* !INPUTNORMALIZER_HPP_ */