/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Button
*/

#include "Button.hpp"
#include <string>
#include "../../../../libs/Multimedia/IWindow.hpp"

namespace ui {

Button::Button(std::shared_ptr<ResourceManager> resourceManager)
    : AFocusableElement(resourceManager) {
}

void Button::render() {
    if (!_visible)
        return;

    AFocusableElement::render();

    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();

    gfx::color_t bgColor = getCurrentColor();
    resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
        bgColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())}
    );

    gfx::color_t borderColor = {0, 0, 0};
    resourceManager->get<gfx::IWindow>()->drawRectangleOutline(
        borderColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())}
    );

    if (!_text.empty()) {
        float textX = absPos.getX() + absSize.getX() / 2.0f;
        float textY = absPos.getY() + absSize.getY() / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawText(
            _text,
            _textColor,
            {static_cast<size_t>(textX), static_cast<size_t>(textY)},
            _fontPath
        );
    }
}

gfx::color_t Button::getCurrentColor() const {
    switch (_state) {
        case UIState::Hovered:
            return _hoveredColor;
        case UIState::Pressed:
            return _pressedColor;
        case UIState::Disabled:
            return _disabledColor;
        case UIState::Focused:
            return _focusedColor;
        default:
            return _normalColor;
    }
}

void Button::setText(const std::string& text) {
    _text = text;
}

const std::string& Button::getText() const {
    return _text;
}

void Button::setTextColor(const gfx::color_t& color) {
    _textColor = color;
}

void Button::setFontPath(const std::string& fontPath) {
    _fontPath = fontPath;
}

void Button::setNormalColor(const gfx::color_t& color) {
    _normalColor = color;
}

void Button::setHoveredColor(const gfx::color_t& color) {
    _hoveredColor = color;
}

void Button::setPressedColor(const gfx::color_t& color) {
    _pressedColor = color;
}

void Button::setDisabledColor(const gfx::color_t& color) {
    _disabledColor = color;
}

void Button::setFocusedColor(const gfx::color_t& color) {
    _focusedColor = color;
}

}  // namespace ui
