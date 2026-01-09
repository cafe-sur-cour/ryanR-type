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
#include <cstdint>
#include "../../common/types/FRect.hpp"
#include "../../common/types/Vector2f.hpp"

namespace gfx {

struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;
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
        virtual void drawText(std::string text, color_t color, std::pair<size_t, size_t> position, const std::string& fontPath, size_t fontSize = 24, color_t outlineColor = {0, 0, 0}, float outlineThickness = 0.0f) = 0;
        virtual std::pair<size_t, size_t> getTextSize(const std::string& text, const std::string& fontPath, size_t fontSize = 24) = 0;
        virtual void drawRectangleOutline(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, size_t borderWidth = 5) = 0;
        virtual void drawFilledRectangle(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual void drawRoundedRectangleFilled(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) = 0;
        virtual void drawRoundedRectangleOutline(color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) = 0;

        virtual bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) = 0;
        virtual std::pair<int, int> getWindowSize() = 0;

        virtual void drawSprite(const std::string& texturePath, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f) = 0;
        virtual void drawSprite(const std::string& texturePath, float x, float y, const math::FRect frameRect, float scaleX = 1.0f, float scaleY = 1.0f, float rotation = 0.0f) = 0;

        virtual void updateView() = 0;
        virtual void setViewCenter(float x, float y) = 0;
        virtual math::Vector2f getViewCenter() = 0;
        virtual math::Vector2f mapPixelToCoords(int x, int y) = 0;

        virtual std::pair<int, int> getLogicalSize() const = 0;
        virtual float getScaleFactor() const = 0;

        virtual void addShaderFilter(const std::string& path) = 0;
        virtual void removeShaderFilter(const std::string& path) = 0;
        virtual void setShaderUniform(const std::string& filterPath, const std::string& name, float value) = 0;
        virtual void setFramerateLimit(unsigned int fps) = 0;
        virtual void setFullscreen(bool fullscreen) = 0;
        virtual void setRenderQuality(float quality) = 0;
        virtual void setCursor(bool isHand) = 0;
};

typedef IWindow *(*createWindow_t)();

} // namespace gfx

#endif /* !IWINDOW_HPP_ */
