/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextComponent
*/

#ifndef TEXTCOMPONENT_HPP_
#define TEXTCOMPONENT_HPP_

#include <string>
#include "../../../common/components/base/IComponent.hpp"
#include "../../../common/interfaces/IWindow.hpp"
#include "../../colors.hpp"

namespace ecs {

class TextComponent : public IComponent {
    public:
        TextComponent(
            const std::string& text,
            const std::string& fontPath,
            gfx::color_t color = colors::WHITE
        ) : _text(text), _fontPath(fontPath), _color(color) {};
        ~TextComponent() {};

        const std::string& getText() const { return _text; }
        const std::string& getFontPath() const { return _fontPath; }
        const gfx::color_t& getColor() const { return _color; }

        void setText(const std::string& text) { _text = text; }
        void setFontPath(const std::string& fontPath) { _fontPath = fontPath; }
        void setColor(const gfx::color_t& color) { _color = color; }

    protected:
    private:
        std::string _text;
        std::string _fontPath;
        gfx::color_t _color;
};

} // namespace ecs

#endif /* !TEXTCOMPONENT_HPP_ */
