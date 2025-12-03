/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextComponent
*/

#include "TextComponent.hpp"

namespace ecs {

TextComponent::TextComponent(const std::string& text, const std::string& fontPath, gfx::color_t color)
    : _text(text), _fontPath(fontPath), _color(color) {
}

TextComponent::~TextComponent() {
}

} // namespace ecs