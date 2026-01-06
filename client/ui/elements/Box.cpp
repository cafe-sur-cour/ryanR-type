/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Box
*/

#include <utility>

#include "Box.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

Box::Box(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager) {
}

void Box::setBackgroundColor(const gfx::color_t& color) {
    _backgroundColor = color;
}

void Box::setBorderColor(const gfx::color_t& color) {
    _borderColor = color;
}

void Box::setBorderThickness(float thickness) {
    _borderThickness = thickness;
}

void Box::setCornerRadius(float radius) {
    _cornerRadius = radius;
}

void Box::render() {
    if (!_visible) {
        return;
    }

    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    auto window = resourceManager->get<gfx::IWindow>();
    if (!window) {
        return;
    }

    math::Vector2f absolutePos = getAbsolutePosition();
    math::Vector2f absoluteSize = getAbsoluteSize();

    // Dessiner le fond du rectangle
    window->drawFilledRectangle(
        _backgroundColor,
        std::make_pair(static_cast<size_t>(absolutePos.getX()),
            static_cast<size_t>(absolutePos.getY())),
        std::make_pair(static_cast<size_t>(absoluteSize.getX()),
            static_cast<size_t>(absoluteSize.getY()))
    );

    // Dessiner la bordure si thickness > 0
    if (_borderThickness > 0) {
        window->drawRectangleOutline(
            _borderColor,
            std::make_pair(static_cast<size_t>(absolutePos.getX()),
                static_cast<size_t>(absolutePos.getY())),
            std::make_pair(static_cast<size_t>(absoluteSize.getX()),
                static_cast<size_t>(absoluteSize.getY()))
        );
    }

    // Render children
    for (auto& child : _children) {
        if (child) {
            child->render();
        }
    }
}

}  // namespace ui
