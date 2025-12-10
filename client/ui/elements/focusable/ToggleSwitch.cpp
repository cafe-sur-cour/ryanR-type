/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ToggleSwitch
*/

#include "ToggleSwitch.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
#include "../../../../common/constants.hpp"

namespace ui {

ToggleSwitch::ToggleSwitch(std::shared_ptr<ResourceManager> resourceManager)
    : AFocusableElement(resourceManager) {
    setOnRelease([this]() {
        setValue(!_value);
    });
    setOnActivated([this]() {
        setValue(!_value);
    });
}

void ToggleSwitch::setValue(bool value) {
    bool oldValue = _value;
    _value = value;
    if (oldValue != _value && _onValueChanged) {
        _onValueChanged(_value);
    }
}

bool ToggleSwitch::getValue() const {
    return _value;
}

void ToggleSwitch::setFontPath(const std::string& fontPath) {
    _fontPath = fontPath;
}

void ToggleSwitch::setBaseFontSize(size_t fontSize) {
    _baseFontSize = fontSize;
}

size_t ToggleSwitch::getBaseFontSize() const {
    return _baseFontSize;
}

void ToggleSwitch::setOnText(const std::string& text) {
    _onText = text;
}

void ToggleSwitch::setOffText(const std::string& text) {
    _offText = text;
}

void ToggleSwitch::setTrackColor(const gfx::color_t& color) {
    _trackColor = color;
}

void ToggleSwitch::setHandleColor(const gfx::color_t& color) {
    _handleColor = color;
}

void ToggleSwitch::setHandleHoveredColor(const gfx::color_t& color) {
    _handleHoveredColor = color;
}

void ToggleSwitch::setHandleFocusedColor(const gfx::color_t& color) {
    _handleFocusedColor = color;
}

void ToggleSwitch::setOnColor(const gfx::color_t& color) {
    _onColor = color;
}

void ToggleSwitch::setOffColor(const gfx::color_t& color) {
    _offColor = color;
}

void ToggleSwitch::setOnValueChanged(std::function<void(bool)> callback) {
    _onValueChanged = callback;
}

void ToggleSwitch::render() {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    auto window = resourceManager->get<gfx::IWindow>();
    if (!window) return;

    math::Vector2f pos = getAbsolutePosition();
    math::Vector2f size = getAbsoluteSize();

    float trackHeight = size.getY();
    float trackWidth = size.getX();
    float trackX = pos.getX() + (size.getX() - trackWidth) / 2.0f;
    float trackY = pos.getY() + (size.getY() - trackHeight) / 2.0f;
    window->drawFilledRectangle(_trackColor,
        {static_cast<size_t>(trackX),static_cast<size_t>(trackY)},
        {static_cast<size_t>(trackWidth), static_cast<size_t>(trackHeight)});

    float handleWidth = trackWidth / 2.0f;
    float handleHeight = trackHeight;
    float handleX = _value ? (trackX + trackWidth - handleWidth) : trackX;
    float handleY = trackY;

    gfx::color_t handleColor = _handleColor;
    if (isFocused()) {
        handleColor = _handleFocusedColor;
    } else if (_isHovered) {
        handleColor = _handleHoveredColor;
    }

    window->drawFilledRectangle(handleColor,
        {static_cast<size_t>(handleX), static_cast<size_t>(handleY)},
        {static_cast<size_t>(handleWidth), static_cast<size_t>(handleHeight)});

    size_t smallFontSize = static_cast<size_t>(static_cast<double>(_baseFontSize) * 0.6);

    auto offTextSize = window->getTextSize(_offText, _fontPath, smallFontSize);
    size_t offTextX = static_cast<size_t>
        (trackX + handleWidth / 2.0f - static_cast<float>(offTextSize.first) / 2.0f);
    size_t offTextY = static_cast<size_t>
        (trackY + trackHeight / 2.0f - static_cast<float>(offTextSize.second) / 2.0f);
    gfx::color_t offTextColor = _value ? gfx::color_t{150, 150, 150} : _offColor;
    window->drawText(_offText, offTextColor, {offTextX, offTextY}, _fontPath, smallFontSize);

    auto onTextSize = window->getTextSize(_onText, _fontPath, smallFontSize);
    size_t onTextX = static_cast<size_t>
        (trackX + trackWidth - handleWidth / 2.0f -
            static_cast<float>(onTextSize.first) / 2.0f);
    size_t onTextY = static_cast<size_t>
        (trackY + trackHeight / 2.0f - static_cast<float>(onTextSize.second) / 2.0f);
    gfx::color_t onTextColor = _value ? _onColor : gfx::color_t{150, 150, 150};
    window->drawText(_onText, onTextColor, {onTextX, onTextY}, _fontPath, smallFontSize);
}

void ToggleSwitch::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    math::Vector2f pos = getAbsolutePosition();
    math::Vector2f size = getAbsoluteSize();

    _isHovered = mousePos.getX() >= pos.getX() && mousePos.getX() <= pos.getX() + size.getX() &&
                 mousePos.getY() >= pos.getY() && mousePos.getY() <= pos.getY() + size.getY();

    AFocusableElement::handleInput(mousePos, mousePressed);
}

bool ToggleSwitch::containsPoint(const math::Vector2f& point) const {
    if (!_visible) {
        return false;
    }

    math::Vector2f pos = getAbsolutePosition();
    math::Vector2f size = getAbsoluteSize();

    return (point.getX() >= pos.getX() &&
            point.getX() <= pos.getX() + size.getX() &&
            point.getY() >= pos.getY() &&
            point.getY() <= pos.getY() + size.getY());
}

}  // namespace ui
