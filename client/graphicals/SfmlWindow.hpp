/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlWindow
*/

#ifndef SFMLWINDOW_HPP_
#define SFMLWINDOW_HPP_

#include <SFML/Graphics.hpp>
#include "./IWindow.hpp"

class SfmlWindow : public gfx::IWindow {
    public:
        SfmlWindow(std::string title = "R-Type", size_t width = 800, size_t height = 600);
        ~SfmlWindow() override;
        void display() override;
        void closeWindow() override;
        bool isOpen() override;
        void clear() override;

        void resizeWindow(size_t x, size_t y) override;

        void drawSprite(std::string asset, gfx::color_t color, std::string text, std::pair<size_t, size_t> position) override;
        void drawText(std::string text, gfx::color_t color, std::pair<size_t, size_t> position) override;
        void drawRectangle(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;

        void setFont(const std::string& fontPath) override;
        std::string getFont() const override;

        bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        std::pair<int, int> getWindowSize() override;
    private:
        sf::RenderWindow _window;
        sf::Font _font;
        std::string _fontPath;
};

#endif /* !SFMLWINDOW_HPP_ */
