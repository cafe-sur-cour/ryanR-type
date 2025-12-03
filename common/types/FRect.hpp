/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** FRect
*/

#ifndef FRECT_HPP_
#define FRECT_HPP_

namespace math {

class FRect {
    public:
        FRect();
        FRect(float left, float top, float width, float height);
        FRect(FRect const &other);
        ~FRect() = default;

        float getLeft() const;
        void setLeft(float left);
        float getTop() const;
        void setTop(float top);
        float getWidth() const;
        void setWidth(float width);
        float getHeight() const;
        void setHeight(float height);

        bool contains(float x, float y) const;
        bool intersects(FRect const &other) const;
        bool intersects(FRect const &other, FRect &intersection) const;

        FRect &operator=(FRect const &other);
        bool operator==(FRect const &other) const;
        bool operator!=(FRect const &other) const;

        private:
            float left;
            float top;
            float width;
            float height;
};

} // namespace math

#endif /* !FRECT_HPP_ */
