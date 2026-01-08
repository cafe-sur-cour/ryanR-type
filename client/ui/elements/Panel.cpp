/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Panel
*/

#include "Panel.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

Panel::Panel(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager) {
}

void Panel::setScale(UIScale scale) {
    (void)scale;
}

void Panel::setBackgroundColor(gfx::color_t color) {
    _backgroundColor = color;
}

void Panel::setBorderColor(gfx::color_t color) {
    _borderColor = color;
}

void Panel::setBorderThickness(float thickness) {
    _borderThickness = thickness;
}

void Panel::render() {
    if (!_visible) return;

    auto rm = _resourceManager.lock();
    if (!rm) return;

    auto window = rm->get<gfx::IWindow>();
    if (!window) return;

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();

    window->drawFilledRectangle(
        _backgroundColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())}
    );

    window->drawRectangleOutline(
        _borderColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())}
    );

    for (auto& child : _children) {
        if (child->isVisible()) {
            child->render();
        }
    }
}

void Panel::update(float deltaTime) {
    for (auto& child : _children) {
        child->update(deltaTime);
    }
}

}  // namespace ui
