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

struct color_t {
    int r;
    int g;
    int b;
};

class IWindow {
    public:
        virtual ~IWindow() = default;
        virtual void display() = 0;
        virtual void closeWindow() = 0;
        virtual bool isOpen() = 0;
        virtual void clear() = 0;

        virtual void setMapSize(std::pair<size_t, size_t> size) = 0;
        virtual void resizeWindow(size_t x, size_t y) = 0;

        virtual void drawSprite(std::string asset, int color, std::string text, std::pair<size_t, size_t> position) = 0;
        virtual void drawText(std::string text, int color, std::pair<size_t, size_t> position) = 0;
        virtual void drawRectangle(int color, std::pair<size_t, size_t> position) = 0;

        virtual bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual std::pair<int, int> getWindowSize() = 0;
};

#endif /* !IWINDOW_HPP_ */
