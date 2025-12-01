/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IWindow
*/

#ifndef IWINDOW_HPP_
#define IWINDOW_HPP_

#include <string>
#include <utility>
#include "../../common/types/FRect.hpp"

namespace gfx {

struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class IWindow {
    public:
        virtual ~IWindow() = default;

        virtual void init() = 0;
        virtual void display() = 0;
        virtual void closeWindow() = 0;
        virtual bool isOpen() = 0;
        virtual void clear() = 0;
        virtual void resizeWindow(size_t x, size_t y) = 0;

        virtual void drawSprite(std::string asset, color_t color, std::pair<size_t, size_t> position) = 0;
        virtual void drawText(std::string text, color_t color, std::pair<size_t, size_t> position, const std::string& fontPath) = 0;
        virtual void drawRectangleOutline(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual void drawFilledRectangle(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;

        virtual bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual std::pair<int, int> getWindowSize() = 0;

        virtual void drawSprite(const std::string& texturePath, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f) = 0;
        virtual void drawSprite(const std::string& texturePath, float x, float y, const math::FRect frameRect, float scaleX = 1.0f, float scaleY = 1.0f) = 0;
};

typedef IWindow *(*createWindow_t)();

} // namespace gfx

#endif /* !IWINDOW_HPP_ */
