/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ToggleSwitch
*/

#ifndef TOGGLESWITCH_HPP_
#define TOGGLESWITCH_HPP_

#include "../../core/AFocusableElement.hpp"
#include <string>
#include <functional>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
#include "../../../colors.hpp"

namespace ui {

class ToggleSwitch : public AFocusableElement {
    public:
        explicit ToggleSwitch(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~ToggleSwitch() = default;

        void setValue(bool value);
        bool getValue() const;

        void setFontPath(const std::string& fontPath);
        void setBaseFontSize(size_t fontSize);
        size_t getBaseFontSize() const;

        void setOnText(const std::string& text);
        void setOffText(const std::string& text);

        void setTrackColor(const gfx::color_t& color);
        void setHandleColor(const gfx::color_t& color);
        void setHandleHoveredColor(const gfx::color_t& color);
        void setHandleFocusedColor(const gfx::color_t& color);
        void setOnColor(const gfx::color_t& color);
        void setOffColor(const gfx::color_t& color);

        void setOnValueChanged(std::function<void(bool)> callback);

        virtual void render() override;
        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed) override;
        virtual bool containsPoint(const math::Vector2f& point) const override;

    private:
        bool _value = false;
        std::string _fontPath = "assets/fonts/ARIAL.TTF";
        size_t _baseFontSize = constants::BUTTON_FONT_SIZE_BASE;
        std::string _onText = "ON";
        std::string _offText = "OFF";

        gfx::color_t _trackColor = colors::TOGGLE_TRACK;
        gfx::color_t _handleColor = colors::TOGGLE_HANDLE;
        gfx::color_t _handleHoveredColor = colors::TOGGLE_HANDLE_HOVER;
        gfx::color_t _handleFocusedColor = colors::TOGGLE_HANDLE_FOCUSED;
        gfx::color_t _onColor = colors::TOGGLE_ON;
        gfx::color_t _offColor = colors::TOGGLE_OFF;

        std::function<void(bool)> _onValueChanged;

        bool _isHovered = false;
};

}  // namespace ui

#endif  // TOGGLESWITCH_HPP_
