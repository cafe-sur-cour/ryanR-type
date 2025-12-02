/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Button
*/

#ifndef BUTTON_HPP_
#define BUTTON_HPP_

#include "UIElement.hpp"
#include <string>
#include "../../libs/Multimedia/IWindow.hpp"

namespace ui {

class Button : public UIElement {
    public:
        Button(std::shared_ptr<ecs::ResourceManager> resourceManager);
        virtual ~Button() = default;

        void setText(const std::string& text);
        const std::string& getText() const;

        void setTextColor(const gfx::color_t& color);
        void setFontPath(const std::string& fontPath);

        void setNormalColor(const gfx::color_t& color);
        void setHoveredColor(const gfx::color_t& color);
        void setPressedColor(const gfx::color_t& color);
        void setDisabledColor(const gfx::color_t& color);

        virtual void render() override;

    private:
        std::string _text;
        gfx::color_t _textColor = {255, 255, 255};
        std::string _fontPath = "assets/fonts/ARIAL.TTF";

        gfx::color_t _normalColor = {100, 100, 100};
        gfx::color_t _hoveredColor = {150, 150, 150};
        gfx::color_t _pressedColor = {200, 200, 200};
        gfx::color_t _disabledColor = {50, 50, 50};

        gfx::color_t getCurrentColor() const;
};

}  // namespace ui

#endif /* !BUTTON_HPP_ */
