/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Slider
*/

#include "Slider.hpp"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
#include "../../../../common/constants.hpp"

namespace ui {

Slider::Slider(std::shared_ptr<ResourceManager> resourceManager)
    : AFocusableElement(resourceManager) {
}

void Slider::setMinValue(float minValue) {
    _minValue = minValue;
    if (_value < _minValue) {
        _value = _minValue;
    }
}

void Slider::setMaxValue(float maxValue) {
    _maxValue = maxValue;
    if (_value > _maxValue) {
        _value = _maxValue;
    }
}

void Slider::setValue(float value) {
    float oldValue = _value;
    _value = std::clamp(value, _minValue, _maxValue);
    if (std::abs(oldValue - _value) > constants::EPS && _onValueChanged) {
        _onValueChanged(_value);
    }
}

float Slider::getValue() const {
    return _value;
}

float Slider::getMinValue() const {
    return _minValue;
}

float Slider::getMaxValue() const {
    return _maxValue;
}

void Slider::setStep(float step) {
    _step = step;
}

float Slider::getStep() const {
    return _step;
}

void Slider::setLabel(const std::string& label) {
    _label = label;
}

const std::string& Slider::getLabel() const {
    return _label;
}

void Slider::setLabelColor(const gfx::color_t& color) {
    _labelColor = color;
}

void Slider::setFontPath(const std::string& fontPath) {
    _fontPath = fontPath;
}

void Slider::setBaseFontSize(size_t fontSize) {
    _baseFontSize = fontSize;
}

size_t Slider::getBaseFontSize() const {
    return _baseFontSize;
}

void Slider::setShowPercentage(bool show) {
    _showPercentage = show;
}

void Slider::setTrackColor(const gfx::color_t& color) {
    _trackColor = color;
}

void Slider::setFillColor(const gfx::color_t& color) {
    _fillColor = color;
}

void Slider::setHandleColor(const gfx::color_t& color) {
    _handleColor = color;
}

void Slider::setHandleHoveredColor(const gfx::color_t& color) {
    _handleHoveredColor = color;
}

void Slider::setHandleFocusedColor(const gfx::color_t& color) {
    _handleFocusedColor = color;
}

void Slider::setOnValueChanged(std::function<void(float)> callback) {
    _onValueChanged = callback;
}

float Slider::getNormalizedValue() const {
    if (std::abs(_maxValue - _minValue) < constants::EPS) {
        return 0.0f;
    }
    return (_value - _minValue) / (_maxValue - _minValue);
}

void Slider::setNormalizedValue(float normalized) {
    normalized = std::clamp(normalized, 0.0f, 1.0f);
    setValue(_minValue + normalized * (_maxValue - _minValue));
}

gfx::color_t Slider::getCurrentHandleColor() const {
    if (_focused) {
        return _handleFocusedColor;
    }
    switch (_state) {
        case UIState::Hovered:
        case UIState::Pressed:
            return _handleHoveredColor;
        default:
            return _handleColor;
    }
}

size_t Slider::getFontSize() const {
    float scale = getScaleFactor();
    return static_cast<size_t>(static_cast<float>(_baseFontSize) * scale);
}

float Slider::getHandleRadius() const {
    return getAbsoluteSize().getY() * 0.15f;
}

float Slider::getTrackHeight() const {
    return getAbsoluteSize().getY() * 0.15f;
}

float Slider::getLabelHeight() const {
    return getAbsoluteSize().getY() * 0.4f;
}

void Slider::render() {
    if (!_visible)
        return;

    AFocusableElement::render();

    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();

    float labelHeight = getLabelHeight();
    float trackHeight = getTrackHeight();
    float handleRadius = getHandleRadius();

    float trackY = absPos.getY() +
        labelHeight + (absSize.getY() - labelHeight) / 2.0f - trackHeight / 2.0f;
    float trackX = absPos.getX() + handleRadius;
    float trackWidth = absSize.getX() - 2.0f * handleRadius;

    resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
        _trackColor,
        {static_cast<size_t>(trackX), static_cast<size_t>(trackY)},
        {static_cast<size_t>(trackWidth), static_cast<size_t>(trackHeight)}
    );

    resourceManager->get<gfx::IWindow>()->drawRectangleOutline(
        colors::UI_OUTLINE,
        {static_cast<size_t>(trackX), static_cast<size_t>(trackY)},
        {static_cast<size_t>(trackWidth), static_cast<size_t>(trackHeight)}
    );

    float fillWidth = trackWidth * getNormalizedValue();
    if (fillWidth > 0) {
        resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
            _fillColor,
            {static_cast<size_t>(trackX), static_cast<size_t>(trackY)},
            {static_cast<size_t>(fillWidth), static_cast<size_t>(trackHeight)}
        );
    }

    float handleX = trackX + fillWidth;
    float handleY = trackY + trackHeight / 2.0f;

    gfx::color_t handleColor = getCurrentHandleColor();
    resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
        handleColor,
        {
            static_cast<size_t>(handleX - handleRadius),
            static_cast<size_t>(handleY - handleRadius)
        },
        {
            static_cast<size_t>(handleRadius * 2.0f),
            static_cast<size_t>(handleRadius * 2.0f)
        }
    );

    resourceManager->get<gfx::IWindow>()->drawRectangleOutline(
        {0, 0, 0},
        {
            static_cast<size_t>(handleX - handleRadius),
            static_cast<size_t>(handleY - handleRadius)
        },
        {
            static_cast<size_t>(handleRadius * 2.0f),
            static_cast<size_t>(handleRadius * 2.0f)
        }
    );

    if (!_label.empty()) {
        std::ostringstream oss;
        std::string suffix = (_showPercentage) ? "%" : "";
        oss << _label << ": " << std::fixed << std::setprecision(0) <<
            (_value * (_showPercentage ? 100.0f : 1.0f)) << suffix;
        std::string displayText = oss.str();

        auto textSize = resourceManager->get<gfx::IWindow>()->getTextSize(
            displayText, _fontPath, getFontSize());
        float textX = absPos.getX() +
            (absSize.getX() - static_cast<float>(textSize.first)) / 2.0f;
        float textY = absPos.getY() +
            (labelHeight - static_cast<float>(textSize.second)) / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawText(
            displayText,
            _labelColor,
            {static_cast<size_t>(textX), static_cast<size_t>(textY)},
            _fontPath,
            getFontSize(),
            colors::UI_OUTLINE,
            _outlineThickness
        );
    }
}

void Slider::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    AFocusableElement::handleInput(mousePos, mousePressed);

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();
    float handleRadius = getHandleRadius();
    float trackX = absPos.getX() + handleRadius;
    float trackWidth = absSize.getX() - 2.0f * handleRadius;

    bool justPressed = mousePressed && !_wasMousePressed;

    if (justPressed && containsPoint(mousePos)) {
        _isDragging = true;
    }

    if (!mousePressed) {
        _isDragging = false;
    }

    if (_isDragging) {
        float normalized = (mousePos.getX() - trackX) / trackWidth;
        setNormalizedValue(normalized);
    }

    _wasMousePressed = mousePressed;
}

void Slider::onActivated() {
    AFocusableElement::onActivated();
}

bool Slider::onNavigateLeft() {
    decrementValue();
    return true;
}

bool Slider::onNavigateRight() {
    incrementValue();
    return true;
}

void Slider::incrementValue() {
    setValue(_value + _step);
}

void Slider::decrementValue() {
    setValue(_value - _step);
}

}  // namespace ui
