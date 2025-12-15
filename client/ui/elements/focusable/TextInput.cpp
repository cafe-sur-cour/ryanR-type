/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextInput
*/

#include "TextInput.hpp"
#include <algorithm>
#include <string>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
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
    _text = text;
    _cursorPosition = _text.length();
    if (_onTextChanged) {
        _onTextChanged(_text);
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

void TextInput::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    AFocusableElement::handleInput(mousePos, mousePressed);
}

void TextInput::handleKeyboardInput(gfx::EventType event) {
    if (!_focused) return;

    switch (event) {
        case gfx::EventType::PERIOD: insertChar('.'); break;
        case gfx::EventType::A: insertChar('a'); break;
        case gfx::EventType::B: insertChar('b'); break;
        case gfx::EventType::C: insertChar('c'); break;
        case gfx::EventType::D: insertChar('d'); break;
        case gfx::EventType::E: insertChar('e'); break;
        case gfx::EventType::F: insertChar('f'); break;
        case gfx::EventType::G: insertChar('g'); break;
        case gfx::EventType::H: insertChar('h'); break;
        case gfx::EventType::I: insertChar('i'); break;
        case gfx::EventType::J: insertChar('j'); break;
        case gfx::EventType::K: insertChar('k'); break;
        case gfx::EventType::L: insertChar('l'); break;
        case gfx::EventType::M: insertChar('m'); break;
        case gfx::EventType::N: insertChar('n'); break;
        case gfx::EventType::O: insertChar('o'); break;
        case gfx::EventType::P: insertChar('p'); break;
        case gfx::EventType::Q: insertChar('q'); break;
        case gfx::EventType::R: insertChar('r'); break;
        case gfx::EventType::S: insertChar('s'); break;
        case gfx::EventType::T: insertChar('t'); break;
        case gfx::EventType::U: insertChar('u'); break;
        case gfx::EventType::V: insertChar('v'); break;
        case gfx::EventType::W: insertChar('w'); break;
        case gfx::EventType::X: insertChar('x'); break;
        case gfx::EventType::Y: insertChar('y'); break;
        case gfx::EventType::Z: insertChar('z'); break;
        case gfx::EventType::NUMPAD0:
        case gfx::EventType::NUM0: insertChar('0'); break;
        case gfx::EventType::NUMPAD1:
        case gfx::EventType::NUM1: insertChar('1'); break;
        case gfx::EventType::NUMPAD2:
        case gfx::EventType::NUM2: insertChar('2'); break;
        case gfx::EventType::NUMPAD3:
        case gfx::EventType::NUM3: insertChar('3'); break;
        case gfx::EventType::NUMPAD4:
        case gfx::EventType::NUM4: insertChar('4'); break;
        case gfx::EventType::NUMPAD5:
        case gfx::EventType::NUM5: insertChar('5'); break;
        case gfx::EventType::NUMPAD6:
        case gfx::EventType::NUM6: insertChar('6'); break;
        case gfx::EventType::NUMPAD7:
        case gfx::EventType::NUM7: insertChar('7'); break;
        case gfx::EventType::NUMPAD8:
        case gfx::EventType::NUM8: insertChar('8'); break;
        case gfx::EventType::NUMPAD9:
        case gfx::EventType::NUM9: insertChar('9'); break;
        case gfx::EventType::SPACE: insertChar(' '); break;
        case gfx::EventType::BACKSPACE: deleteChar(); break;
        case gfx::EventType::ENTER:
            if (_onSubmit) _onSubmit(_text);
            break;
        case gfx::EventType::LEFT: moveCursorLeft(); break;
        case gfx::EventType::RIGHT: moveCursorRight(); break;
        default: break;
    }
}

void TextInput::insertChar(char c) {
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
