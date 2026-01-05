/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RoundedRectangleShape
*/

#ifndef ROUNDED_RECTANGLE_SHAPE_HPP_
#define ROUNDED_RECTANGLE_SHAPE_HPP_

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace sf {

class RoundedRectangleShape : public Shape {
public:
    RoundedRectangleShape();
    RoundedRectangleShape(const Vector2f& size, float radius = 0.f, unsigned int cornerPointCount = 8);

    void setSize(const Vector2f& size);
    const Vector2f& getSize() const;

    void setRadius(float radius);
    float getRadius() const;

    void setCornerPointCount(unsigned int count);
    unsigned int getCornerPointCount() const;

    virtual std::size_t getPointCount() const override;
    virtual Vector2f getPoint(std::size_t index) const override;

private:
    Vector2f _size;
    float _radius;
    unsigned int _cornerPointCount;
};

} // namespace sf

#endif /* !ROUNDED_RECTANGLE_SHAPE_HPP_ */
