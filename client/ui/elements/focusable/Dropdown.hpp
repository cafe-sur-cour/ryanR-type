/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Dropdown
*/

#ifndef DROPDOWN_HPP_
#define DROPDOWN_HPP_

#include "../../core/AFocusableElement.hpp"
#include <string>
#include <vector>
#include <functional>
#include "../../../../common/interfaces/IWindow.hpp"
#include "../../../colors.hpp"
#include "../../../constants.hpp"

namespace ui {

enum class DropdownDirection {
    Down,
    Up,
    Left,
    Right
};

class Dropdown : public AFocusableElement {
    public:
        Dropdown(std::shared_ptr<ResourceManager> resourceManager);
        ~Dropdown();

        virtual void render() override;
        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed) override;
        virtual void update(float deltaTime) override;
        virtual bool containsPoint(const math::Vector2f& point) const override;

        void setOptions(const std::vector<std::string>& options);
        void addOption(const std::string& option);
        void clearOptions();

        const std::vector<std::string>& getOptions() const;

        void setSelectedIndex(size_t index);
        size_t getSelectedIndex() const;
        const std::string& getSelectedValue() const;

        void setDirection(DropdownDirection direction);
        DropdownDirection getDirection() const;

        void setPlaceholder(const std::string& placeholder);
        const std::string& getPlaceholder() const;

        void setTextColor(const gfx::color_t& color);
        void setPlaceholderColor(const gfx::color_t& color);
        void setFontPath(const std::string& fontPath);
        void setBaseFontSize(size_t fontSize);
        size_t getBaseFontSize() const;

        void setOnSelectionChanged(std::function<void(const std::string&, size_t)> callback);

        void open();
        void close();
        bool isOpen() const;

    private:
        std::vector<std::string> _options;
        size_t _selectedIndex = 0;
        bool _hasSelection = false;

        DropdownDirection _direction = DropdownDirection::Down;
        bool _isOpen = false;
        int _hoveredOptionIndex = -1;

        std::string _placeholder = "Select...";

        std::function<void(const std::string&, size_t)> _onSelectionChanged;

        gfx::color_t _textColor = colors::UI_TEXT;
        gfx::color_t _placeholderColor = colors::UI_DISABLED;
        std::string _fontPath = constants::MAIN_FONT;
        size_t _baseFontSize = constants::BUTTON_FONT_SIZE_BASE;

        gfx::color_t _normalColor = colors::BUTTON_PRIMARY;
        gfx::color_t _hoveredColor = colors::BUTTON_PRIMARY_HOVER;
        gfx::color_t _pressedColor = colors::BUTTON_PRIMARY_PRESSED;
        gfx::color_t _disabledColor = colors::UI_DISABLED;
        gfx::color_t _focusedColor = colors::UI_FOCUSED;

        void renderClosed();
        void renderOpen();
        bool isMouseOverOption(const math::Vector2f& mousePos, size_t optionIndex) const;
        math::Vector2f getOptionPosition(size_t optionIndex) const;
        math::Vector2f getDropdownSize() const;
        size_t getFontSize() const;
        gfx::color_t _getCurrentColor() const;

        bool _dropdownWasPressed = false;
};

}  // namespace ui

#endif /* !DROPDOWN_HPP_ */
