/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Button
*/

#include "Button.hpp"
#include <string>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"

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
    resourceManager->get<gfx::IWindow>()->drawRoundedRectangleFilled(
        bgColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
        15.0f
    );

    gfx::color_t borderColor = {0, 0, 0};
    resourceManager->get<gfx::IWindow>()->drawRoundedRectangleOutline(
        borderColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
        15.0f
    );

    if (!_iconPath.empty() && _iconSize.getX() > 0.0f && _iconSize.getY() > 0.0f) {
        float margin = 5.0f * getScaleFactor();
        float targetWidth = absSize.getX() - (2.0f * margin);
        float targetHeight = absSize.getY() - (2.0f * margin);

        float scaleX = targetWidth / _iconSize.getX();
        float scaleY = targetHeight / _iconSize.getY();

        float iconX = absPos.getX() + margin;
        float iconY = absPos.getY() + margin;

        resourceManager->get<gfx::IWindow>()->drawSprite(
            _iconPath,
            iconX,
            iconY,
            scaleX,
            scaleY
        );
    } else if (!_text.empty()) {
        auto textSize = resourceManager->get<
            gfx::IWindow>()->getTextSize(_text, _fontPath, getFontSize());
        float textX = absPos.getX() +
            (absSize.getX() - static_cast<float>(textSize.first)) / 2.0f;
        float textY = absPos.getY() +
            (absSize.getY() - static_cast<float>(textSize.second)) / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawText(
            _text,
            _textColor,
            {static_cast<size_t>(textX), static_cast<size_t>(textY)},
            _fontPath,
            getFontSize()
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

void Button::setBaseFontSize(size_t fontSize) {
    _baseFontSize = fontSize;
}

size_t Button::getBaseFontSize() const {
    return _baseFontSize;
}

size_t Button::getFontSize() const {
    float scale = getScaleFactor();
    return static_cast<size_t>(static_cast<float>(_baseFontSize) * scale);
}

void Button::setIconPath(const std::string& iconPath) {
    _iconPath = iconPath;
}

void Button::setIconSize(const math::Vector2f& size) {
    _iconSize = size;
}

}  // namespace ui
