/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlWindow
*/

#ifndef SFMLWINDOW_HPP_
#define SFMLWINDOW_HPP_

#include <memory>
#include <SFML/Graphics.hpp>
#include "IWindow.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "../../common/types/FRect.hpp"
#include "AssetManager/AssetManager.hpp"

class SfmlWindow : public gfx::IWindow {
    public:
        SfmlWindow(std::string title = "R-Type", size_t width = 800, size_t height = 600);
        ~SfmlWindow() override;
        void init() override;
        void display() override;
        void closeWindow() override;
        bool isOpen() override;
        void clear() override;
        void resizeWindow(size_t x, size_t y) override;

        void drawSprite(std::string asset, gfx::color_t color, std::pair<size_t, size_t> position) override;
        void drawText(std::string text, gfx::color_t color, std::pair<size_t, size_t> position, const std::string& fontPath) override;
        void drawRectangleOutline(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        void drawFilledRectangle(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;

        bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        std::pair<int, int> getWindowSize() override;

        void drawSprite(const std::string& texturePath, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f) override;
        void drawSprite(const std::string& texturePath, float x, float y, const math::FRect frameRect, float scaleX = 1.0f, float scaleY = 1.0f) override;
        std::shared_ptr<sf::RenderWindow> getSfmlWindow();

    private:
        std::shared_ptr<sf::RenderWindow> _window;
        std::shared_ptr<assets::AssetManager> _assetManager;
        gfx::TextureManager _textureManager;
        gfx::FontManager _fontManager;
};

#endif /* !SFMLWINDOW_HPP_ */
