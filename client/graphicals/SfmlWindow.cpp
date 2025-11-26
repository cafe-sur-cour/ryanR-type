/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlWindow
*/

#include "SfmlWindow.hpp"
#include <stdexcept>
#include <iostream>
#include <utility>
#include <string>
#include <memory>

SfmlWindow::SfmlWindow(std::string title, size_t width, size_t height)
    : _window(std::make_shared<sf::RenderWindow>(
    sf::VideoMode({static_cast<unsigned int>(width),
    static_cast<unsigned int>(height)}), title)),
    _fontPath("./assets/fonts/ARIAL.TTF") {
    if (!_font.openFromFile(_fontPath)) {
        std::cout << "Failed to load font: " << _fontPath << std::endl;
        // throw std::runtime_error("Failed to load font: " + _fontPath);
    }
    init();
}

SfmlWindow::~SfmlWindow() {
    if (_window->isOpen())
        _window->close();
}

void SfmlWindow::init() {
    bool isActive = false;

    _window->setFramerateLimit(60);
    _window->clear(sf::Color::Black);
    _window->clear();
    _window->display();
    isActive = _window->setActive(true);
    (void)isActive;
}

void SfmlWindow::display() {
    _window->display();
}

void SfmlWindow::closeWindow() {
    _window->close();
}

bool SfmlWindow::isOpen() {
    return _window->isOpen();
}

void SfmlWindow::clear() {
    _window->clear();
}

void SfmlWindow::resizeWindow(size_t x, size_t y) {
    _window->setSize(sf::Vector2u(
        static_cast<unsigned int>(x),
        static_cast<unsigned int>(y) ));
}
void SfmlWindow::drawSprite(std::string asset, gfx::color_t color,
    std::pair<size_t, size_t> position) {
    sf::Texture texture;
    if (!texture.loadFromFile(asset))
        throw std::runtime_error("Failed to load texture: " + asset);
    sf::Sprite sprite(texture);
    sprite.setColor(sf::Color(color.r, color.g, color.b));
    sprite.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));
    _window->draw(sprite);
}

void SfmlWindow::drawText(std::string text, gfx::color_t color,
    std::pair<size_t, size_t> position) {
    sf::Text sfText(_font, sf::String(text), 24);
    sfText.setFillColor(sf::Color(color.r, color.g, color.b));
    sfText.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));
    _window->draw(sfText);
}

void SfmlWindow::drawRectangle(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)));
    rectangle.setFillColor(sf::Color(color.r, color.g, color.b));
    rectangle.setPosition(sf::Vector2f(static_cast<float>(position.first),
    static_cast<float>(position.second)));
    _window->draw(rectangle);
}

bool SfmlWindow::isMouseOver(std::pair<size_t, size_t> position,
    std::pair<size_t, size_t> size) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
    sf::IntRect rect(sf::Vector2i(static_cast<int>(position.first),
        static_cast<int>(position.second)),
        sf::Vector2i(static_cast<int>(size.first),
        static_cast<int>(size.second)));
    return rect.contains(mousePos);
}

std::pair<int, int> SfmlWindow::getWindowSize() {
    sf::Vector2u size = _window->getSize();
    return {static_cast<int>(size.x), static_cast<int>(size.y)};
}

void SfmlWindow::setFont(const std::string& fontPath) {
    _fontPath = fontPath;
    if (!_font.openFromFile(_fontPath))
        throw std::runtime_error("Failed to load font: " + _fontPath);
}

std::string SfmlWindow::getFont() const {
    return _fontPath;
}

std::shared_ptr<sf::RenderWindow> SfmlWindow::getSfmlWindow() {
    return _window;
}
