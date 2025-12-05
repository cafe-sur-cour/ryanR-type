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
#include "../../common/constants.hpp"

SfmlWindow::SfmlWindow(std::string title, size_t width, size_t height)
    : _window(std::make_shared<sf::RenderWindow>(
    sf::VideoMode({static_cast<unsigned int>(width),
    static_cast<unsigned int>(height)}), title, sf::Style::Default)),
    _assetManager(std::make_shared<assets::AssetManager>()),
    _textureManager(_assetManager),
    _fontManager(_assetManager),
    _shaderManager(_assetManager),
    _view(sf::FloatRect(sf::Vector2f(0.f, 0.f),
        sf::Vector2f(constants::MAX_WIDTH, constants::MAX_HEIGHT))),
    _renderTexture({static_cast<unsigned int>(constants::MAX_WIDTH),
        static_cast<unsigned int>(constants::MAX_HEIGHT)}),
    _renderSprite(_renderTexture.getTexture()) {
    init();
}

SfmlWindow::~SfmlWindow() {
    if (_window->isOpen())
        _window->close();
}

void SfmlWindow::init() {
    bool isActive = false;

    _window->setFramerateLimit(60);
    _renderTexture.setView(_view);

    updateView();
    _window->clear(sf::Color::Black);
    _window->clear();
    _window->display();
    isActive = _window->setActive(true);
    (void)isActive;
}

void SfmlWindow::display() {
    _renderTexture.display();

    sf::Vector2u windowSize = _window->getSize();
    sf::View defaultView(sf::FloatRect(sf::Vector2f(0.f, 0.f),
        sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y))));
    defaultView.setViewport(_view.getViewport());

    _window->clear();
    _window->setView(defaultView);

    auto activeShaders = _shaderManager.getActiveShaders();
    if (activeShaders.empty()) {
        _renderSprite.setTexture(_renderTexture.getTexture());
        _window->draw(_renderSprite);
    } else if (activeShaders.size() == 1) {
        _renderSprite.setTexture(_renderTexture.getTexture());
        _window->draw(_renderSprite, activeShaders[0].get());
    } else {
        sf::RenderTexture intermediate({static_cast<unsigned int>(constants::MAX_WIDTH),
                                       static_cast<unsigned int>(constants::MAX_HEIGHT)});

        sf::Sprite tempSprite(_renderTexture.getTexture());
        intermediate.clear();
        intermediate.draw(tempSprite, activeShaders[0].get());
        intermediate.display();

        for (size_t i = 1; i < activeShaders.size(); ++i) {
            sf::RenderTexture nextIntermediate(
                {static_cast<unsigned int>(constants::MAX_WIDTH),
                static_cast<unsigned int>(constants::MAX_HEIGHT)});

            sf::Sprite intermediateSprite(intermediate.getTexture());
            nextIntermediate.clear();
            nextIntermediate.draw(intermediateSprite, activeShaders[i].get());
            nextIntermediate.display();

            intermediate = std::move(nextIntermediate);
        }

        sf::Sprite finalSprite(intermediate.getTexture());
        _window->draw(finalSprite);
    }

    _window->display();
}

void SfmlWindow::closeWindow() {
    _window->close();
}

bool SfmlWindow::isOpen() {
    return _window->isOpen();
}

void SfmlWindow::clear() {
    _renderTexture.clear();
}

void SfmlWindow::resizeWindow(size_t x, size_t y) {
    _window->setSize(sf::Vector2u(
        static_cast<unsigned int>(x),
        static_cast<unsigned int>(y) ));
}
void SfmlWindow::drawSprite(std::string asset, gfx::color_t color,
    std::pair<size_t, size_t> position) {
    auto texture = _textureManager.loadTexture(asset);
    if (!texture)
        throw std::runtime_error("Failed to load texture: " + asset);

    sf::Sprite sprite(*texture);
    sprite.setColor(sf::Color(color.r, color.g, color.b, color.a));
    sprite.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));
    _renderTexture.draw(sprite);
}

void SfmlWindow::drawText(std::string text, gfx::color_t color,
    std::pair<size_t, size_t> position, const std::string& fontPath) {
    auto font = _fontManager.getFont(fontPath);
    if (!font) {
        std::cout << "Failed to load font: " << fontPath << std::endl;
        return;
    }

    sf::Text sfText(*font, sf::String(text), 24);
    sfText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    sfText.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));
    _renderTexture.draw(sfText);
}

void SfmlWindow::drawRectangleOutline(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(sf::Vector2f(static_cast<float>(position.first),
    static_cast<float>(position.second)));
    _renderTexture.draw(rectangle);
}

void SfmlWindow::drawFilledRectangle(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)));
    rectangle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    rectangle.setPosition(sf::Vector2f(static_cast<float>(position.first),
    static_cast<float>(position.second)));
    _renderTexture.draw(rectangle);
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
        return drawFilledRectangle({255, 0, 0}, {static_cast<size_t>(x),
            static_cast<size_t>(y)}, {50, 50});

    sf::Sprite sprite(*texture);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    _renderTexture.draw(sprite);
}

void SfmlWindow::drawSprite(const std::string& texturePath, float x, float y,
    const math::FRect frameRect, float scaleX, float scaleY) {
    auto texture = _textureManager.loadTexture(texturePath);
    if (!texture)
        return drawFilledRectangle({255, 0, 0}, {static_cast<size_t>(x),
            static_cast<size_t>(y)}, {50, 50});


    sf::Sprite sprite(*texture);
    sf::IntRect textureRect(
        sf::Vector2i(static_cast<int>(frameRect.getLeft()),
            static_cast<int>(frameRect.getTop())),
        sf::Vector2i(static_cast<int>(frameRect.getWidth()),
            static_cast<int>(frameRect.getHeight())));
    sprite.setTextureRect(textureRect);
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
    _renderTexture.draw(sprite);
}

void SfmlWindow::updateView() {
    sf::Vector2u windowSize = _window->getSize();
    float windowRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
    float viewRatio = constants::MAX_WIDTH / constants::MAX_HEIGHT;

    sf::FloatRect viewport;
    if (windowRatio > viewRatio) {
        float scale = static_cast<float>(windowSize.y) / constants::MAX_HEIGHT;
        float viewportWidth = (constants::MAX_WIDTH * scale) /
            static_cast<float>(windowSize.x);
        viewport = sf::FloatRect(sf::Vector2f((1.f - viewportWidth) / 2.f, 0.f),
            sf::Vector2f(viewportWidth, 1.f));
    } else {
        float scale = static_cast<float>(windowSize.x) / constants::MAX_WIDTH;
        float viewportHeight = (constants::MAX_HEIGHT * scale) /
            static_cast<float>(windowSize.y);
        viewport = sf::FloatRect(sf::Vector2f(0.f, (1.f - viewportHeight) / 2.f),
            sf::Vector2f(1.f, viewportHeight));
    }

    _view.setViewport(viewport);
}

void SfmlWindow::setViewCenter(float x, float y) {
    _view.setCenter(sf::Vector2f(x, y));
    _renderTexture.setView(_view);
}

math::Vector2f SfmlWindow::getViewCenter() {
    sf::Vector2f center = _view.getCenter();
    return math::Vector2f(center.x, center.y);
}

math::Vector2f SfmlWindow::mapPixelToCoords(int x, int y) {
    sf::Vector2i pixelPos(x, y);
    sf::Vector2f worldPos = _window->mapPixelToCoords(pixelPos);
    return math::Vector2f(worldPos.x, worldPos.y);
}

void SfmlWindow::enableFilter(const std::string& filterName) {
    _shaderManager.enableFilter(filterName);
}

void SfmlWindow::addFilter(const std::string& filterName) {
    _shaderManager.addFilter(filterName);
}

void SfmlWindow::removeFilter(const std::string& filterName) {
    _shaderManager.removeFilter(filterName);
}

void SfmlWindow::disableAllFilters() {
    _shaderManager.disableAllFilters();
}

bool SfmlWindow::isFilterActive(const std::string& filterName) const {
    return _shaderManager.isFilterActive(filterName);
}

ShaderManager& SfmlWindow::getShaderManager() {
    return _shaderManager;
}
