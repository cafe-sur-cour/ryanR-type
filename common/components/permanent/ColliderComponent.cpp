/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ColliderComponent
*/

#include "ColliderComponent.hpp"
#include <algorithm>
#include <limits>
#include <vector>

namespace ecs {

math::Vector2f ColliderComponent::getOffset() const {
    return _offset;
}

void ColliderComponent::setOffset(math::Vector2f offset) {
    _offset = offset;
}

math::Vector2f ColliderComponent::getSize() const {
    return _size;
}

void ColliderComponent::setSize(math::Vector2f size) {
    _size = size;
}

CollisionType ColliderComponent::getType() const {
    return _type;
}

void ColliderComponent::setType(CollisionType type) {
    _type = type;
}

math::FRect ColliderComponent::getHitbox(math::Vector2f entityPosition,
    math::Vector2f scale) const {
    return math::FRect(entityPosition.getX() + _offset.getX(), entityPosition.getY() +
        _offset.getY(), _size.getX() * scale.getX(), _size.getY() * scale.getY());
}

math::FRect ColliderComponent::getScaledHitbox(math::Vector2f entityPosition,
    math::Vector2f scale) const {
    return getHitbox(entityPosition, scale);
}

math::OrientedRect ColliderComponent::getOrientedHitbox(math::Vector2f entityPosition,
    math::Vector2f scale, float rotation) const {
    math::Vector2f scaledSize = math::Vector2f(_size.getX() *
        scale.getX(), _size.getY() * scale.getY());
    math::Vector2f topLeft = entityPosition + _offset;
    math::Vector2f center = topLeft + scaledSize * 0.5f;
    return math::OrientedRect(center, scaledSize, rotation);
}

math::FRect ColliderComponent::getHitbox(math::Vector2f entityPosition,
    math::Vector2f scale, float rotation) const {
    if (std::abs(rotation) < 0.1f) {
        return getHitbox(entityPosition, scale);
    }
    double theta = static_cast<double>(rotation) * acos(-1.0) / 180.0;
    double cos_theta = std::cos(theta);
    double sin_theta = std::sin(theta);
    math::Vector2f scaled_size = math::Vector2f(_size.getX() *
        scale.getX(), _size.getY() * scale.getY());
    math::Vector2f top_left = entityPosition + _offset;
    math::Vector2f center = top_left + scaled_size * 0.5f;
    math::Vector2f half_size = scaled_size * 0.5f;
    std::vector<math::Vector2f> relativeCorners = {
        math::Vector2f(-half_size.getX(), -half_size.getY()),
        math::Vector2f(half_size.getX(), -half_size.getY()),
        math::Vector2f(half_size.getX(), half_size.getY()),
        math::Vector2f(-half_size.getX(), half_size.getY())
    };
    float min_x = std::numeric_limits<float>::max();
    float max_x = std::numeric_limits<float>::lowest();
    float min_y = std::numeric_limits<float>::max();
    float max_y = std::numeric_limits<float>::lowest();
    for (auto& rel : relativeCorners) {
        double rel_x = static_cast<double>(rel.getX());
        double rel_y = static_cast<double>(rel.getY());
        double rx = rel_x * cos_theta - rel_y * sin_theta;
        double ry = rel_x * sin_theta + rel_y * cos_theta;
        float world_x = center.getX() + static_cast<float>(rx);
        float world_y = center.getY() + static_cast<float>(ry);
        min_x = std::min(min_x, world_x);
        max_x = std::max(max_x, world_x);
        min_y = std::min(min_y, world_y);
        max_y = std::max(max_y, world_y);
    }
    return math::FRect(min_x, min_y, max_x - min_x, max_y - min_y);
}

}  // namespace ecs
