/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Button
*/

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "../../core/AFocusableElement.hpp"
#include <string>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
#include "../../../colors.hpp"

namespace ui {

class Button : public AFocusableElement {
    public:
        explicit Button(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~Button() = default;

        void setText(const std::string& text);
        const std::string& getText() const;
        void setTextColor(const gfx::color_t& color);
        void setFontPath(const std::string& fontPath);

        void setNormalColor(const gfx::color_t& color);
        void setHoveredColor(const gfx::color_t& color);
        void setPressedColor(const gfx::color_t& color);
        void setDisabledColor(const gfx::color_t& color);
        void setFocusedColor(const gfx::color_t& color);
        void setBaseFontSize(size_t fontSize);
        size_t getBaseFontSize() const;

        virtual void render() override;

    private:
        std::string _text;
        gfx::color_t _textColor = colors::UI_TEXT;
        std::string _fontPath = "assets/fonts/arial.ttf";

        gfx::color_t _normalColor = colors::BUTTON_PRIMARY;
        gfx::color_t _hoveredColor = colors::BUTTON_PRIMARY_HOVER;
        gfx::color_t _pressedColor = colors::BUTTON_PRIMARY_PRESSED;
        gfx::color_t _disabledColor = colors::UI_DISABLED;
        gfx::color_t _focusedColor = colors::UI_FOCUSED;
        size_t _baseFontSize = constants::BUTTON_FONT_SIZE_BASE;

        gfx::color_t getCurrentColor() const;
        size_t getFontSize() const;
};

}  // namespace ui

#endif /* !BUTTON_HPP_ */
