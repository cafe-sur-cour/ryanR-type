/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** IWindow
*/


#ifndef IWINDOW_HPP_
#define IWINDOW_HPP_

#include <string>
#include <utility>
#include <memory>

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
        virtual void drawText(std::string text, color_t color, std::pair<size_t, size_t> position) = 0;
        virtual void drawRectangle(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;

        virtual void setFont(const std::string& fontPath) = 0;
        virtual std::string getFont() const = 0;

        virtual bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual std::pair<int, int> getWindowSize() = 0;
};

} // namespace gfx

#endif /* !IWINDOW_HPP_ */
