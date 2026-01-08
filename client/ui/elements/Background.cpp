/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Background
*/

#include "Background.hpp"
#include <algorithm>
#include <string>
#include "../../../common/constants.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

Background::Background(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager) {
}

void Background::addLayer(
    const std::string& texturePath,
    float speedX,
    float speedY,
    const math::Vector2f& sourceSize
) {
    _layers.push_back({texturePath, speedX, speedY, sourceSize, 0.0f, 0.0f});
}

void Background::update(float deltaTime) {
    const float screenWidth = constants::MAX_WIDTH;
    const float screenHeight = constants::MAX_HEIGHT;

    for (auto& layer : _layers) {
        layer.offsetX += layer.speedX * deltaTime;
        layer.offsetY += layer.speedY * deltaTime;

        if (layer.offsetX > screenWidth) layer.offsetX -= screenWidth;
        if (layer.offsetX < -screenWidth) layer.offsetX += screenWidth;
        if (layer.offsetY > screenHeight) layer.offsetY -= screenHeight;
        if (layer.offsetY < -screenHeight) layer.offsetY += screenHeight;
    }
}

float Background::calculateScale(const Layer& layer, float screenWidth) {
    const float screenHeight = constants::MAX_HEIGHT;
    float scaleX = screenWidth / layer.sourceSize.getX();
    float scaleY = screenHeight / layer.sourceSize.getY();
    return (std::max)(scaleX, scaleY);
}

void Background::render() {
    if (!_visible) return;

    auto rm = _resourceManager.lock();
    if (!rm) return;

    auto window = rm->get<gfx::IWindow>();
    if (!window) return;

    const float screenWidth = constants::MAX_WIDTH;

    for (const auto& layer : _layers) {
        float scale = calculateScale(layer, screenWidth);

        float x = layer.offsetX;
        float y = layer.offsetY;
        float effectiveWidth = layer.sourceSize.getX() * scale;

        window->drawSprite(layer.texturePath, x, y, scale, scale);

        if (x < 0)
            window->drawSprite(layer.texturePath, x + effectiveWidth, y, scale, scale);
        if (x + effectiveWidth > screenWidth)
            window->drawSprite(layer.texturePath, x - effectiveWidth, y, scale, scale);
    }
}

}  // namespace ui
