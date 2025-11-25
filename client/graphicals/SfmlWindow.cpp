/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlWindow
*/

#include "SfmlWindow.hpp"
#include <stdexcept>

SfmlWindow::SfmlWindow(std::string title, size_t width, size_t height)
    : _window(sf::RenderWindow(sf::VideoMode(width, height), title)), _fontPath("assets/font.ttf") {
    if (!_font.loadFromFile(_fontPath))
        throw std::runtime_error("Failed to load font: " + _fontPath);
}

SfmlWindow::~SfmlWindow() {
    if (_window.isOpen())
        _window.close();
}

void SfmlWindow::display() {
    _window.display();
}

void SfmlWindow::closeWindow() {
    _window.close();
}

bool SfmlWindow::isOpen() {
    return _window.isOpen();
}

void SfmlWindow::clear() {
    _window.clear();
}

void SfmlWindow::resizeWindow(size_t x, size_t y) {
    _window.setSize(sf::Vector2u(x, y));
}
void SfmlWindow::drawSprite(std::string asset, gfx::color_t color, std::string text, std::pair<size_t, size_t> position) {
    sf::Texture texture;
    if (!texture.loadFromFile(asset))
        throw std::runtime_error("Failed to load texture: " + asset);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setColor(sf::Color(color.r, color.g, color.b));
    sprite.setPosition(static_cast<float>(position.first), static_cast<float>(position.second));
    _window.draw(sprite);
}

void SfmlWindow::drawText(std::string text, gfx::color_t color, std::pair<size_t, size_t> position) {
    sf::Text sfText;
    sfText.setFont(_font);
    sfText.setString(text);
    sfText.setCharacterSize(24);
    sfText.setFillColor(sf::Color(color.r, color.g, color.b));
    sfText.setPosition(static_cast<float>(position.first), static_cast<float>(position.second));
    _window.draw(sfText);
}

void SfmlWindow::drawRectangle(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first), static_cast<float>(size.second)));
    rectangle.setFillColor(sf::Color(color.r, color.g, color.b));
    rectangle.setPosition(static_cast<float>(position.first), static_cast<float>(position.second));
    _window.draw(rectangle);
}

bool SfmlWindow::isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);
    sf::IntRect rect(static_cast<int>(position.first), static_cast<int>(position.second),
        static_cast<int>(size.first), static_cast<int>(size.second));
    return rect.contains(mousePos);
}

std::pair<int, int> SfmlWindow::getWindowSize() {
    sf::Vector2u size = _window.getSize();
    return {static_cast<int>(size.x), static_cast<int>(size.y)};
}

void SfmlWindow::setFont(const std::string& fontPath) {
    _fontPath = fontPath;
    if (!_font.loadFromFile(_fontPath))
        throw std::runtime_error("Failed to load font: " + _fontPath);
}

std::string SfmlWindow::getFont() const {
    return _fontPath;
}