/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Vector2f
*/

#include "Vector2f.hpp"

Vector2f::Vector2f(float x, float y) : _x(x), _y(y) {
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
    _x = other._x;
    _y = other._y;
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
