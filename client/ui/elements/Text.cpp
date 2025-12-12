/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Text
*/

#include "Text.hpp"
#include <string>
#include "../../../libs/Multimedia/IWindow.hpp"

namespace ui {

Text::Text(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager), _text(""), _textColor({255, 255, 255}), _fontSize(24),
_baseFontSize(24), _fontPath("assets/fonts/arial.ttf"
) {
}

void Text::setScale(UIScale scale) {
    UIElement::setScale(scale);
    float scaleFactor = getScaleFactor();
    _fontSize = static_cast<unsigned int>(static_cast<float>(_baseFontSize)
        * scaleFactor + 0.5f);
}

void Text::render() {
    if (!_visible || _text.empty())
        return;

    auto resourceMgr = _resourceManager.lock();
    if (!resourceMgr) return;

    auto window = resourceMgr->get<gfx::IWindow>();
    math::Vector2f absolutePos = getAbsolutePosition();
    window->drawText(
        _text,
        _textColor,
        {static_cast<size_t>(absolutePos.getX()), static_cast<size_t>(absolutePos.getY())},
        _fontPath,
        _fontSize
    );
}

void Text::update(float deltaTime) {
    (void)deltaTime;
}

void Text::setText(const std::string& text) {
    _text = text;
}

std::string Text::getText() const {
    return _text;
}

void Text::setTextColor(const gfx::color_t& color) {
    _textColor = color;
}

void Text::setFontSize(unsigned int size) {
    _baseFontSize = size;
    _fontSize = size;
}

void Text::setFontPath(const std::string& path) {
    _fontPath = path;
}

}  // namespace ui
