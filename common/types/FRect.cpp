/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** FRect
*/

#include "FRect.hpp"
#include <algorithm>

namespace math {

FRect::FRect()
    : left(0.0f)
    , top(0.0f)
    , width(0.0f)
    , height(0.0f) {
}

FRect::FRect(float rectLeft, float rectTop, float rectWidth, float rectHeight)
    : left(rectLeft)
    , top(rectTop)
    , width(rectWidth)
    , height(rectHeight) {
}

FRect::FRect(FRect const &other)
    : left(other.left)
    , top(other.top)
    , width(other.width)
    , height(other.height) {
}

float FRect::getLeft() const {
    return left;
}

void FRect::setLeft(float rectLeft) {
    this->left = rectLeft;
}

float FRect::getTop() const {
    return top;
}

void FRect::setTop(float rectTop) {
    this->top = rectTop;
}

float FRect::getWidth() const {
    return width;
}

void FRect::setWidth(float rectWidth) {
    this->width = rectWidth;
}

float FRect::getHeight() const {
    return height;
}

void FRect::setHeight(float rectHeight) {
    this->height = rectHeight;
}

bool FRect::contains(float x, float y) const {
    float minX = std::min(left, left + width);
    float maxX = std::max(left, left + width);
    float minY = std::min(top, top + height);
    float maxY = std::max(top, top + height);

    return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
}

bool FRect::intersects(FRect const &other) const {
    FRect intersection;
    return intersects(other, intersection);
}

bool FRect::intersects(FRect const &other, FRect &intersection) const {
    float r1MinX = std::min(left, left + width);
    float r1MaxX = std::max(left, left + width);
    float r1MinY = std::min(top, top + height);
    float r1MaxY = std::max(top, top + height);

    float r2MinX = std::min(other.left, other.left + other.width);
    float r2MaxX = std::max(other.left, other.left + other.width);
    float r2MinY = std::min(other.top, other.top + other.height);
    float r2MaxY = std::max(other.top, other.top + other.height);

    float interLeft = std::max(r1MinX, r2MinX);
    float interTop = std::max(r1MinY, r2MinY);
    float interRight = std::min(r1MaxX, r2MaxX);
    float interBottom = std::min(r1MaxY, r2MaxY);

    if ((interLeft < interRight) && (interTop < interBottom)) {
        intersection = FRect(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    } else {
        intersection = FRect(0, 0, 0, 0);
        return false;
    }
}

FRect &FRect::operator=(FRect const &other) {
    if (this != &other) {
        left = other.left;
        top = other.top;
        width = other.width;
        height = other.height;
    }
    return *this;
}

bool FRect::operator==(FRect const &other) const {
    return (left == other.left) && (top == other.top) &&
           (width == other.width) && (height == other.height);
}

bool FRect::operator!=(FRect const &other) const {
    return !(*this == other);
}

} // namespace math
