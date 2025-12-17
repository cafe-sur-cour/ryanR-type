/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Vector2f
*/

#include <cmath>
#include <iostream>

#include "Vector2f.hpp"

namespace math {

Vector2f::Vector2f(float x, float y) : _x(x), _y(y) {
}

Vector2f::Vector2f(Vector2f const &other) {
    if (std::isnan(other._x) || std::isnan(other._y) || std::isinf(other._x)
        || std::isinf(other._y)) {
        _x = 0.f;
        _y = 0.f;
    } else {
        _x = other._x;
        _y = other._y;
    }
}


float Vector2f::getX() const {
    return _x;
}

void Vector2f::setX(float x) {
    _x = x;
}

float Vector2f::getY() const {
    return _y;
}

void Vector2f::setY(float y) {
    _y = y;
}

Vector2f Vector2f::getVector() const {
    return Vector2f(_x, _y);
}

Vector2f Vector2f::operator*(float scalar) const {
    return Vector2f(_x * scalar, _y * scalar);
}

Vector2f Vector2f::operator-(Vector2f const &other) const {
    return Vector2f(_x - other._x, _y - other._y);
}

Vector2f Vector2f::operator+(Vector2f const &other) const {
    return Vector2f(_x + other._x, _y + other._y);
}

void Vector2f::operator=(Vector2f const &other) {
    if (this != &other) {
        if (std::isnan(other._x) || std::isnan(other._y) || std::isinf(other._x)
        || std::isinf(other._y)) {
            std::cout << "[Vector2f] Warning: Assignment operator received invalid Vector2f"
                      << " (x: " << other._x << ", y: " << other._y << ")\n";
            _x = 0.f;
            _y = 0.f;
        } else {
            _x = other._x;
            _y = other._y;
        }
    }
}

void Vector2f::operator+=(Vector2f const &other) {
    _x += other._x;
    _y += other._y;
}

void Vector2f::operator-=(Vector2f const &other) {
    _x -= other._x;
    _y -= other._y;
}

void Vector2f::operator*=(float scalar) {
    _x *= scalar;
    _y *= scalar;
}

void Vector2f::operator/=(float scalar) {
    _x /= scalar;
    _y /= scalar;
}
}  // namespace math
