/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OrientedRect
*/

#include "OrientedRect.hpp"

namespace math {

OrientedRect::OrientedRect() : _center(0, 0), _size(0, 0), _rotation(0) {}

OrientedRect::OrientedRect(Vector2f center, Vector2f size, float rotation)
    : _center(center), _size(size), _rotation(rotation) {}

OrientedRect::OrientedRect(OrientedRect const &other)
    : _center(other._center), _size(other._size), _rotation(other._rotation) {}

Vector2f OrientedRect::getCenter() const { return _center; }
void OrientedRect::setCenter(Vector2f center) { _center = center; }

Vector2f OrientedRect::getSize() const { return _size; }
void OrientedRect::setSize(Vector2f size) { _size = size; }

float OrientedRect::getRotation() const { return _rotation; }
void OrientedRect::setRotation(float rotation) { _rotation = rotation; }

std::vector<Vector2f> OrientedRect::getCorners() const {
    double theta = static_cast<double>(_rotation) * acos(-1.0) / 180.0;
    double cos_theta = std::cos(theta);
    double sin_theta = std::sin(theta);

    Vector2f halfSize = _size * 0.5f;

    std::vector<Vector2f> relativeCorners = {
        Vector2f(-halfSize.getX(), -halfSize.getY()),
        Vector2f(halfSize.getX(), -halfSize.getY()),
        Vector2f(halfSize.getX(), halfSize.getY()),
        Vector2f(-halfSize.getX(), halfSize.getY())
    };

    std::vector<Vector2f> corners;
    for (auto& rel : relativeCorners) {
        double rel_x = static_cast<double>(rel.getX());
        double rel_y = static_cast<double>(rel.getY());
        double rx = rel_x * cos_theta - rel_y * sin_theta;
        double ry = rel_x * sin_theta + rel_y * cos_theta;
        corners.push_back(_center + Vector2f(static_cast<float>(rx), static_cast<float>(ry)));
    }
    return corners;
}

Vector2f OrientedRect::getAxisX() const {
    double theta = static_cast<double>(_rotation) * acos(-1.0) / 180.0;
    return Vector2f(static_cast<float>(std::cos(theta)), static_cast<float>(std::sin(theta)));
}

Vector2f OrientedRect::getAxisY() const {
    double theta = static_cast<double>(_rotation) * acos(-1.0) / 180.0;
    return Vector2f(static_cast<float>(-std::sin(theta)), static_cast<float>(std::cos(theta)));
}

float OrientedRect::projectPoint(Vector2f point, Vector2f axis) const {
    return point.getX() * axis.getX() + point.getY() * axis.getY();
}

bool OrientedRect::overlapOnAxis(OrientedRect const &other, Vector2f axis) const {
    auto corners1 = getCorners();
    auto corners2 = other.getCorners();

    float min1 = std::numeric_limits<float>::max();
    float max1 = std::numeric_limits<float>::lowest();
    for (auto& p : corners1) {
        float proj = projectPoint(p, axis);
        min1 = std::min(min1, proj);
        max1 = std::max(max1, proj);
    }

    float min2 = std::numeric_limits<float>::max();
    float max2 = std::numeric_limits<float>::lowest();
    for (auto& p : corners2) {
        float proj = projectPoint(p, axis);
        min2 = std::min(min2, proj);
        max2 = std::max(max2, proj);
    }

    return !(max1 < min2 || max2 < min1);
}

bool OrientedRect::intersects(OrientedRect const &other) const {
    if (!overlapOnAxis(other, getAxisX()) ||
        !overlapOnAxis(other, getAxisY()) ||
        !overlapOnAxis(other, other.getAxisX()) ||
        !overlapOnAxis(other, other.getAxisY()))
        return false;
    return true;
}

OrientedRect &OrientedRect::operator=(OrientedRect const &other) {
    _center = other._center;
    _size = other._size;
    _rotation = other._rotation;
    return *this;
}

} // namespace math