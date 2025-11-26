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
    _assetManager(std::make_shared<assets::AssetManager>()),
    _textureManager(_assetManager),
    _fontManager(_assetManager) {
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
    _window->setSize(sf::Vector2u(x, y));
}
void SfmlWindow::drawSprite(std::string asset, gfx::color_t color,
    std::pair<size_t, size_t> position) {
    auto texture = _textureManager.loadTexture(asset);
    if (!texture)
        throw std::runtime_error("Failed to load texture: " + asset);
    sf::Sprite sprite(*texture);
    sprite.setColor(sf::Color(color.r, color.g, color.b));
    sprite.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));
    _window->draw(sprite);
}

void SfmlWindow::drawText(std::string text, gfx::color_t color,
    std::pair<size_t, size_t> position, const std::string& fontPath) {
    auto font = _fontManager.getFont(fontPath);
    if (!font) {
        std::cout << "Failed to load font: " << fontPath << std::endl;
        return;
    }
    sf::Text sfText(*font, sf::String(text), 24);
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

std::shared_ptr<sf::RenderWindow> SfmlWindow::getSfmlWindow() {
    return _window;
}

void SfmlWindow::drawSprite(const std::string& texturePath,
    float x, float y, float scaleX, float scaleY) {
    auto texture = _textureManager.loadTexture(texturePath);
    if (!texture)
        return drawRectangle({255, 0, 0}, {static_cast<size_t>(x),
            static_cast<size_t>(y)}, {50, 50});

    sf::Sprite sprite(*texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    _window->draw(sprite);
}
