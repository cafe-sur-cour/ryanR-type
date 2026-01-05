/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OrientedRect
*/

#ifndef ORIENTEDRECT_HPP_
#define ORIENTEDRECT_HPP_

#include "Vector2f.hpp"
#include <vector>
#include <cmath>

namespace math {

class OrientedRect {
    public:
        OrientedRect();
        OrientedRect(Vector2f center, Vector2f size, float rotation);
        OrientedRect(OrientedRect const &other);
        ~OrientedRect() = default;

        Vector2f getCenter() const;
        void setCenter(Vector2f center);
        Vector2f getSize() const;
        void setSize(Vector2f size);
        float getRotation() const;
        void setRotation(float rotation);

        std::vector<Vector2f> getCorners() const;
        Vector2f getAxisX() const;
        Vector2f getAxisY() const;

        bool intersects(OrientedRect const &other) const;

    OrientedRect &operator=(OrientedRect const &other);    private:
        Vector2f _center;
        Vector2f _size;
        float _rotation;

        float projectPoint(Vector2f point, Vector2f axis) const;
        bool overlapOnAxis(OrientedRect const &other, Vector2f axis) const;
};

} // namespace math

#endif /* !ORIENTEDRECT_HPP_ */