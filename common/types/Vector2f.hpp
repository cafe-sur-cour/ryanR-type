/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Vector2f
*/

#ifndef VECTOR2F_HPP_
#define VECTOR2F_HPP_

namespace math {

class Vector2f {
    public:
        Vector2f(float x = 0.0f, float y = 0.0f);
        Vector2f(Vector2f const &other);
        ~Vector2f() = default;

        float getX() const;
        void setX(float x);
        float getY() const;
        void setY(float y);

        Vector2f getVector() const;
        Vector2f operator*(float scalar) const;
        Vector2f operator-(Vector2f const &other) const;
        Vector2f operator+(Vector2f const &other) const;
        void operator=(Vector2f const &other);
        void operator+=(Vector2f const &other);
        void operator-=(Vector2f const &other);
        void operator*=(float scalar);
        void operator/=(float scalar);
    private:
        float _x;
        float _y;
};

} // namespace math

#endif /* !VECTOR2F_HPP_ */
