/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextInput
*/

#ifndef TEXTINPUT_HPP_
#define TEXTINPUT_HPP_

#include "../../core/AFocusableElement.hpp"
#include <string>
#include <functional>
#include "../../../../libs/Multimedia/EventTypes.hpp"
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../colors.hpp"

namespace ui {

class TextInput : public AFocusableElement {
    public:
        TextInput(std::shared_ptr<ResourceManager> resourceManager);
        ~TextInput();

        virtual void render() override;

        void setText(const std::string& text);
        const std::string& getText() const;
        void setPlaceholder(const std::string& placeholder);
        const std::string& getPlaceholder() const;
        void setTextColor(const gfx::color_t& color);
        void setPlaceholderColor(const gfx::color_t& color);
        void setFontPath(const std::string& fontPath);
        void setBaseFontSize(size_t fontSize);
        size_t getBaseFontSize() const;

        void setOnTextChanged(std::function<void(const std::string&)> callback);
        void setOnSubmit(std::function<void(const std::string&)> callback);

        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed) override;
        void handleKeyboardInput(gfx::EventType event);

        virtual void update(float deltaTime) override;

    private:
        std::string _text;
        std::string _placeholder;
        size_t _cursorPosition = 0;
        float _cursorBlinkTimer = 0.0f;
        bool _showCursor = true;

        gfx::color_t _textColor = {0, 0, 0};
        gfx::color_t _placeholderColor = {128, 128, 128};
        std::string _fontPath = "assets/fonts/abduction2002.ttf";
        size_t _baseFontSize = 24;

        std::function<void(const std::string&)> _onTextChanged;
        std::function<void(const std::string&)> _onSubmit;

        void insertChar(char c);
        void deleteChar();
        void moveCursorLeft();
        void moveCursorRight();
        size_t getFontSize() const;
        void updateCursorBlink(float deltaTime);

        gfx::color_t _normalColor = colors::WHITE;
        gfx::color_t _hoveredColor = colors::LIGHT_GRAY;
        gfx::color_t _pressedColor = colors::DARK_GRAY;
        gfx::color_t _disabledColor = colors::UI_DISABLED;
        gfx::color_t _focusedColor = colors::UI_FOCUSED;
        gfx::color_t _getCurrentColor() const;
};

}

#endif /* !TEXTINPUT_HPP_ */
