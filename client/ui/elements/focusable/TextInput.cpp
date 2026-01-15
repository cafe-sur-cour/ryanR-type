/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextInput
*/

#include "TextInput.hpp"
#include <algorithm>
#include <string>
#include "../../../../common/interfaces/IWindow.hpp"
#include "../../../../libs/Multimedia/EventTypes.hpp"

namespace ui {

TextInput::TextInput(std::shared_ptr<ResourceManager> resourceManager)
    : AFocusableElement(resourceManager) {
}

TextInput::~TextInput() {
}

void TextInput::render() {
    if (!_visible)
        return;

    AFocusableElement::render();

    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();

    gfx::color_t bgColor = _getCurrentColor();
    resourceManager->get<gfx::IWindow>()->drawRoundedRectangleFilled(
        bgColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
        5.0f
    );

    gfx::color_t borderColor = _focused ?
        colors::UI_ACCENT : colors::UI_DISABLED;
    resourceManager->get<gfx::IWindow>()->drawRoundedRectangleOutline(
        borderColor,
        {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
        {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
        5.0f
    );

    std::string displayText = _text.empty() ? _placeholder : _text;
    gfx::color_t textColor = _text.empty() ? _placeholderColor : _textColor;

    if (!displayText.empty()) {
        auto textSize = resourceManager->get<gfx::IWindow>()->getTextSize(
            displayText, _fontPath, getFontSize()
        );
        float textX = absPos.getX() + 10.0f;
        float textY = absPos.getY() + (
            absSize.getY() - static_cast<float>(textSize.second)
        ) / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawText(
            displayText,
            textColor,
            {static_cast<size_t>(textX), static_cast<size_t>(textY)},
            _fontPath,
            getFontSize()
        );
    }

    if (_focused && !_text.empty() && _showCursor) {
        std::string textBeforeCursor = _text.substr(0, _cursorPosition);
        auto textSize = resourceManager->get<gfx::IWindow>()->getTextSize(
            textBeforeCursor, _fontPath, getFontSize()
        );
        float cursorX = absPos.getX() + 12.0f + static_cast<float>(textSize.first);
        float cursorY = absPos.getY() + (
            absSize.getY() - static_cast<float>(textSize.second)
        ) / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
            colors::BLACK,
            {static_cast<size_t>(cursorX), static_cast<size_t>(cursorY)},
            {2, static_cast<size_t>(static_cast<double>(textSize.second) * 1.5)}
        );
    }
}

void TextInput::setText(const std::string& text) {
    if (_maxLength > 0 && text.length() > _maxLength) {
        _text = text.substr(0, _maxLength);
    } else {
        _text = text;
    }
    _cursorPosition = _text.length();
    if (_onTextChanged) {
        _onTextChanged(_text);
    }
}

void TextInput::setMaxLength(size_t maxLength) {
    _maxLength = maxLength;
    if (_maxLength > 0 && _text.length() > _maxLength) {
        _text = _text.substr(0, _maxLength);
        _cursorPosition = (std::min)(_cursorPosition, _text.length());
    }
}

const std::string& TextInput::getText() const {
    return _text;
}

void TextInput::setPlaceholder(const std::string& placeholder) {
    _placeholder = placeholder;
}

const std::string& TextInput::getPlaceholder() const {
    return _placeholder;
}

void TextInput::setTextColor(const gfx::color_t& color) {
    _textColor = color;
}

void TextInput::setPlaceholderColor(const gfx::color_t& color) {
    _placeholderColor = color;
}

void TextInput::setFontPath(const std::string& fontPath) {
    _fontPath = fontPath;
}

void TextInput::setBaseFontSize(size_t fontSize) {
    _baseFontSize = fontSize;
}

size_t TextInput::getBaseFontSize() const {
    return _baseFontSize;
}

void TextInput::setOnTextChanged(std::function<void(const std::string&)> callback) {
    _onTextChanged = callback;
}

void TextInput::setOnSubmit(std::function<void(const std::string&)> callback) {
    _onSubmit = callback;
}

void TextInput::setOnFocusLost(std::function<void()> callback) {
    _onFocusLost = callback;
}

void TextInput::setOnNavigate(std::function<void(bool up)> callback) {
    _onNavigate = callback;
}

void TextInput::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    AFocusableElement::handleInput(mousePos, mousePressed);
}

void TextInput::handleKeyboardInput(gfx::EventType event) {
    if (!_focused) return;

    switch (event) {
        case gfx::EventType::BACKSPACE: deleteChar(); break;
        case gfx::EventType::ENTER:
            if (_onSubmit) _onSubmit(_text);
            break;
        case gfx::EventType::NUMPAD_ENTER:
            if (_onSubmit) _onSubmit(_text);
            break;
        case gfx::EventType::LEFT: moveCursorLeft(); break;
        case gfx::EventType::RIGHT: moveCursorRight(); break;
        case gfx::EventType::UP:
            if (_onNavigate) _onNavigate(true);
            break;
        case gfx::EventType::DOWN:
            if (_onNavigate) _onNavigate(false);
            break;
        default: break;
    }
}

void TextInput::handleTextInput(const std::string& text) {
    if (!_focused) return;
    for (char c : text) {
        if (c >= 32 && c <= 126) {
            insertChar(c);
        }
    }
}

void TextInput::insertChar(char c) {
    if (_maxLength > 0 && _text.length() >= _maxLength) {
        return;
    }
    _text.insert(_cursorPosition, 1, c);
    _cursorPosition++;
    if (_onTextChanged) {
        _onTextChanged(_text);
    }
}

void TextInput::deleteChar() {
    if (_cursorPosition > 0) {
        _text.erase(_cursorPosition - 1, 1);
        _cursorPosition--;
        if (_onTextChanged) {
            _onTextChanged(_text);
        }
    }
}

void TextInput::moveCursorLeft() {
    if (_cursorPosition > 0) {
        _cursorPosition--;
    }
}

void TextInput::moveCursorRight() {
    if (_cursorPosition < _text.length()) {
        _cursorPosition++;
    }
}

size_t TextInput::getFontSize() const {
    float scale = getScaleFactor();
    return static_cast<size_t>(static_cast<float>(_baseFontSize) * scale);
}

void TextInput::update(float deltaTime) {
    UIElement::update(deltaTime);
    updateCursorBlink(deltaTime);
}

void TextInput::updateCursorBlink(float deltaTime) {
    _cursorBlinkTimer += deltaTime;
    if (_cursorBlinkTimer >= 0.5f) {
        _showCursor = !_showCursor;
        _cursorBlinkTimer = 0.0f;
    }
}

gfx::color_t TextInput::_getCurrentColor() const {
    switch (_state) {
        case UIState::Hovered:
            return _hoveredColor;
        case UIState::Pressed:
            return _pressedColor;
        case UIState::Focused:
            return _focusedColor;
        default:
            return _normalColor;
    }
}

}  // namespace ui
