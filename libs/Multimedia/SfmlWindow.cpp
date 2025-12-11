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
    _renderSprite(_renderTexture.getTexture()),
    _viewportOffset(0.f, 0.f),
    _viewportScale(1.f, 1.f),
    _renderQuality(1.0f),
    _renderWidth(static_cast<unsigned int>(constants::MAX_WIDTH)),
    _renderHeight(static_cast<unsigned int>(constants::MAX_HEIGHT)),
    _cursorArrow(sf::Cursor::Type::Arrow),
    _cursorHand(sf::Cursor::Type::Hand) {
    init();
}

SfmlWindow::~SfmlWindow() {
    if (_window->isOpen())
        _window->close();
}

void SfmlWindow::init() {
    bool isActive = false;

    _renderTexture.setSmooth(false);
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
        _renderSprite.setPosition(_viewportOffset);
        _renderSprite.setScale(_viewportScale);
        _window->draw(_renderSprite);
    } else if (activeShaders.size() == 1) {
        _renderSprite.setTexture(_renderTexture.getTexture());
        _renderSprite.setPosition(_viewportOffset);
        _renderSprite.setScale(_viewportScale);
        _window->draw(_renderSprite, activeShaders[0].get());
    } else {
        sf::RenderTexture intermediate({_renderWidth, _renderHeight});

        sf::Sprite tempSprite(_renderTexture.getTexture());
        intermediate.clear();
        intermediate.draw(tempSprite, activeShaders[0].get());
        intermediate.display();

        for (size_t i = 1; i < activeShaders.size(); ++i) {
            sf::RenderTexture nextIntermediate({_renderWidth, _renderHeight});

            sf::Sprite intermediateSprite(intermediate.getTexture());
            nextIntermediate.clear();
            nextIntermediate.draw(intermediateSprite, activeShaders[i].get());
            nextIntermediate.display();

            intermediate = std::move(nextIntermediate);
        }

        sf::Sprite finalSprite(intermediate.getTexture());
        finalSprite.setPosition(_viewportOffset);
        finalSprite.setScale(_viewportScale);
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
    _window->clear();
}

void SfmlWindow::resizeWindow(size_t x, size_t y) {
    sf::VideoMode mode(sf::Vector2u(static_cast<unsigned int>(x),
        static_cast<unsigned int>(y)));
    _window->create(mode, "R-Type", sf::Style::Default);
    updateView();
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
    std::pair<size_t, size_t> position, const std::string& fontPath, size_t fontSize,
    gfx::color_t outlineColor, float outlineThickness) {
    auto font = _fontManager.getFont(fontPath);
    if (!font) {
        std::cout << "Failed to load font: " << fontPath << std::endl;
        return;
    }

    sf::Text sfText(*font, sf::String(text), static_cast<unsigned int>(fontSize));
    sfText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    sfText.setPosition(sf::Vector2f(static_cast<float>(position.first),
        static_cast<float>(position.second)));

    if (outlineThickness > 0.0f) {
        sfText.setOutlineColor(
            sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a));
        sfText.setOutlineThickness(outlineThickness);
    }

    _renderTexture.draw(sfText);
}

std::pair<size_t, size_t> SfmlWindow::getTextSize(
    const std::string& text,
    const std::string& fontPath,
    size_t fontSize
) {
    auto font = _fontManager.getFont(fontPath);
    if (!font) {
        std::cout << "Failed to load font: " << fontPath << std::endl;
        return {0, 0};
    }

    sf::Text sfText(*font, sf::String(text), static_cast<unsigned int>(fontSize));
    sf::Rect<float> bounds = sfText.getGlobalBounds();
    sf::Vector2f size = bounds.size;
    return {static_cast<size_t>(size.x), static_cast<size_t>(size.y)};
}

void SfmlWindow::drawRectangleOutline(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) {
    sf::RectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
    rectangle.setOutlineThickness(5.0f);
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

void SfmlWindow::drawRoundedRectangleFilled(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) {
    sf::RoundedRectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)), radius);
    rectangle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    rectangle.setPosition(sf::Vector2f(static_cast<float>(position.first),
    static_cast<float>(position.second)));
    _renderTexture.draw(rectangle);
}

void SfmlWindow::drawRoundedRectangleOutline(gfx::color_t color,
    std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) {
    sf::RoundedRectangleShape rectangle(sf::Vector2f(static_cast<float>(size.first),
    static_cast<float>(size.second)), radius);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
    rectangle.setOutlineThickness(5.0f);
    rectangle.setPosition(sf::Vector2f(static_cast<float>(position.first),
    static_cast<float>(position.second)));
    _renderTexture.draw(rectangle);
}

bool SfmlWindow::isMouseOver(std::pair<size_t, size_t> position,
    std::pair<size_t, size_t> size) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);

    math::Vector2f logicalPos = mapPixelToCoords(mousePos.x, mousePos.y);

    sf::IntRect rect(sf::Vector2i(static_cast<int>(position.first),
        static_cast<int>(position.second)),
        sf::Vector2i(static_cast<int>(size.first),
        static_cast<int>(size.second)));
    return rect.contains(sf::Vector2i(static_cast<int>(logicalPos.getX()),
        static_cast<int>(logicalPos.getY())));
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
            static_cast<size_t>(y)}, {static_cast<size_t>(50), static_cast<size_t>(50)});

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
            static_cast<size_t>(y)}, {static_cast<size_t>(50), static_cast<size_t>(50)});


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

    float scaleX = static_cast<float>(windowSize.x) / static_cast<float>(_renderWidth);
    float scaleY = static_cast<float>(windowSize.y) / static_cast<float>(_renderHeight);

    _view.setViewport(sf::FloatRect(sf::Vector2f(0.f, 0.f), sf::Vector2f(1.f, 1.f)));
    _viewportOffset = sf::Vector2f(0.f, 0.f);
    _viewportScale = sf::Vector2f(scaleX, scaleY);
}

void SfmlWindow::setViewCenter(float x, float y) {
    _view.setCenter(sf::Vector2f(x, y));
    _renderTexture.setView(_view);
    _window->setView(_view);
}

math::Vector2f SfmlWindow::getViewCenter() {
    sf::Vector2f center = _view.getCenter();
    return math::Vector2f(center.x, center.y);
}

math::Vector2f SfmlWindow::mapPixelToCoords(int x, int y) {
    sf::Vector2f pixelPos(static_cast<float>(x), static_cast<float>(y));

    sf::Vector2u windowSize = _window->getSize();

    pixelPos -= _viewportOffset;

    float scaleX = static_cast<float>(windowSize.x) / constants::MAX_WIDTH;
    float scaleY = static_cast<float>(windowSize.y) / constants::MAX_HEIGHT;

    if (scaleX > 0.f && scaleY > 0.f) {
        pixelPos.x *= (1.f / scaleX);
        pixelPos.y *= (1.f / scaleY);
    }

    return math::Vector2f(pixelPos.x, pixelPos.y);
}

std::pair<int, int> SfmlWindow::getLogicalSize() const {
    return {static_cast<int>(constants::MAX_WIDTH), static_cast<int>(constants::MAX_HEIGHT)};
}

float SfmlWindow::getScaleFactor() const {
    if (_viewportScale.x > 0.f) {
        return _viewportScale.x;
    }
    return 1.f;
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

void SfmlWindow::addShaderFilter(const std::string& path) {
    _shaderManager.addFilter(path);
}

void SfmlWindow::removeShaderFilter(const std::string& path) {
    _shaderManager.removeFilter(path);
}

void SfmlWindow::setShaderUniform(
    const std::string& filterPath,
    const std::string& name,
    float value
) {
    _shaderManager.setUniform(filterPath, name, value);
}

void SfmlWindow::setFramerateLimit(unsigned int fps) {
    _window->setFramerateLimit(fps);
}

void SfmlWindow::setFullscreen(bool fullscreen) {
    if (fullscreen) {
        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        _window->create(mode, "R-Type", sf::State::Fullscreen);
        updateView();
    } else {
        sf::VideoMode mode(sf::Vector2u(static_cast<unsigned int>(constants::MAX_WIDTH),
            static_cast<unsigned int>(constants::MAX_HEIGHT)));
        _window->create(mode, "R-Type", sf::Style::Default);
        updateView();
    }
}

void SfmlWindow::setRenderQuality(float quality) {
    if (quality < 0.25f)
        quality = 0.25f;
    if (quality > 1.0f)
        quality = 1.0f;

    _renderQuality = quality;

    _renderWidth = static_cast<unsigned int>(constants::MAX_WIDTH * quality);
    _renderHeight = static_cast<unsigned int>(constants::MAX_HEIGHT * quality);

    if (_renderWidth == 0) _renderWidth = 1;
    if (_renderHeight == 0) _renderHeight = 1;

    if (!_renderTexture.resize({_renderWidth, _renderHeight})) {
        _renderTexture = sf::RenderTexture({_renderWidth, _renderHeight});
    }
    _renderTexture.setSmooth(false);

    _view.setSize(sf::Vector2f(constants::MAX_WIDTH, constants::MAX_HEIGHT));
    _view.setCenter(sf::Vector2f(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f));
    _renderTexture.setView(_view);

    _renderSprite.setTexture(_renderTexture.getTexture());

    updateView();
}

void SfmlWindow::setCursor(bool isHand) {
    if (isHand) {
        _window->setMouseCursor(_cursorHand);
    } else {
        _window->setMouseCursor(_cursorArrow);
    }
}
