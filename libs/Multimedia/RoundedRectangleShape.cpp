/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RoundedRectangleShape
*/

#include "RoundedRectangleShape.hpp"
#include <cmath>

namespace sf {

RoundedRectangleShape::RoundedRectangleShape() :
    _size(0.f, 0.f),
    _radius(0.f),
    _cornerPointCount(8) {
}

RoundedRectangleShape::RoundedRectangleShape(
    const Vector2f& size,
    float radius,
    unsigned int cornerPointCount
) :
    _size(size),
    _radius(radius),
    _cornerPointCount(cornerPointCount) {
    update();
}

void RoundedRectangleShape::setSize(const Vector2f& size) {
    _size = size;
    update();
}

const Vector2f& RoundedRectangleShape::getSize() const {
    return _size;
}

void RoundedRectangleShape::setRadius(float radius) {
    _radius = radius;
    update();
}

float RoundedRectangleShape::getRadius() const {
    return _radius;
}

void RoundedRectangleShape::setCornerPointCount(unsigned int count) {
    _cornerPointCount = count;
    update();
}

unsigned int RoundedRectangleShape::getCornerPointCount() const {
    return _cornerPointCount;
}

std::size_t RoundedRectangleShape::getPointCount() const {
    return 8 + _cornerPointCount * 4;
}

Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    if (index >= getPointCount())
        return Vector2f(0.f, 0.f);

    static const float pi = static_cast<float>(M_PI);
    std::size_t pointIndex = 0;

    if (index == pointIndex++) return Vector2f(_radius, 0.f);
    if (index == pointIndex++) return Vector2f(_size.x - _radius, 0.f);

    for (std::size_t i = 0; i < _cornerPointCount; ++i) {
        if (index == pointIndex++) {
            float t = static_cast<float>(i) / static_cast<float>(_cornerPointCount - 1);
            float angle = 270.f + t * 90.f;
            if (angle >= 360.f) angle -= 360.f;
            return Vector2f(
                _size.x - _radius + std::cos(angle * pi / 180.f) * _radius,
                _radius + std::sin(angle * pi / 180.f) * _radius
            );
        }
    }

    if (index == pointIndex++) return Vector2f(_size.x, _radius);
    if (index == pointIndex++) return Vector2f(_size.x, _size.y - _radius);

    for (std::size_t i = 0; i < _cornerPointCount; ++i) {
        if (index == pointIndex++) {
            float angle =
            (static_cast<float>(i) * 90.f / static_cast<float>(_cornerPointCount - 1));
            return Vector2f(
                _size.x - _radius + std::cos(angle * pi / 180.f) * _radius,
                _size.y - _radius + std::sin(angle * pi / 180.f) * _radius
            );
        }
    }

    if (index == pointIndex++) return Vector2f(_size.x - _radius, _size.y);
    if (index == pointIndex++) return Vector2f(_radius, _size.y);

    for (std::size_t i = 0; i < _cornerPointCount; ++i) {
        if (index == pointIndex++) {
            float angle = 90.f +
            (static_cast<float>(i) * 90.f / static_cast<float>(_cornerPointCount - 1));
            return Vector2f(
                _radius + std::cos(angle * pi / 180.f) * _radius,
                _size.y - _radius + std::sin(angle * pi / 180.f) * _radius
            );
        }
    }

    if (index == pointIndex++) return Vector2f(0.f, _size.y - _radius);
    if (index == pointIndex++) return Vector2f(0.f, _radius);

    for (std::size_t i = 0; i < _cornerPointCount; ++i) {
        if (index == pointIndex++) {
            float angle = 180.f +
                (static_cast<float>(i) * 90.f / static_cast<float>(_cornerPointCount - 1));
            return Vector2f(
                _radius + std::cos(angle * pi / 180.f) * _radius,
                _radius + std::sin(angle * pi / 180.f) * _radius
            );
        }
    }

    return Vector2f(0.f, 0.f);
}

}  // namespace sf
