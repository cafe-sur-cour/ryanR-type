/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Image UI Element
*/

#include <string>
#include "Image.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

Image::Image(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager) {
}

void Image::setTexturePath(const std::string& path) {
    _texturePath = path;
}

void Image::update(float) {
    // No animation needed for static images
}

void Image::render() {
    if (!_visible) return;

    auto rm = _resourceManager.lock();
    if (!rm) return;

    auto window = rm->get<gfx::IWindow>();
    if (!window) return;

    if (_texturePath.empty()) return;

    math::Vector2f absPos = getAbsolutePosition();
    window->drawSprite(_texturePath, absPos.getX(), absPos.getY(), 1.0f, 1.0f);
}

}  // namespace ui
