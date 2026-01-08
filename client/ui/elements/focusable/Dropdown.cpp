/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Dropdown
*/

#include "Dropdown.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include "../../../../common/interfaces/IWindow.hpp"

namespace ui {

Dropdown::Dropdown(std::shared_ptr<ResourceManager> resourceManager)
    : AFocusableElement(resourceManager) {
}

Dropdown::~Dropdown() {
}

void Dropdown::render() {
    if (!_visible) {
        return;
    }

    AFocusableElement::render();

    if (_isOpen) {
        renderOpen();
    } else {
        renderClosed();
    }
}

void Dropdown::renderClosed() {
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

    std::string displayText = _hasSelection ? _options[_selectedIndex] : _placeholder;
    gfx::color_t textColor = _hasSelection ? _textColor : _placeholderColor;

    if (!displayText.empty()) {
        auto textSize = resourceManager->get<gfx::IWindow>()->getTextSize(
            displayText, _fontPath, getFontSize()
        );
        float textX = absPos.getX() + 10.0f;
        float textY =
            absPos.getY() + (absSize.getY() - static_cast<float>(textSize.second)) / 2.0f;

        resourceManager->get<gfx::IWindow>()->drawText(
            displayText,
            textColor,
            {static_cast<size_t>(textX), static_cast<size_t>(textY)},
            _fontPath,
            getFontSize()
        );
    }

    float arrowSize = 8.0f;
    float arrowX = absPos.getX() + absSize.getX() - arrowSize - 10.0f;
    float arrowY = absPos.getY() + (absSize.getY() - arrowSize) / 2.0f;

    drawArrow(arrowX, arrowY, arrowSize);
}

void Dropdown::renderOpen() {
    renderClosed();

    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    for (size_t i = 0; i < _options.size(); ++i) {
        math::Vector2f optionPos = getOptionPosition(i);
        math::Vector2f optionSize = {getAbsoluteSize().getX(), getAbsoluteSize().getY()};

        bool isHovered = (static_cast<int>(i) == _hoveredOptionIndex);
        gfx::color_t optionBgColor =
            isHovered ? colors::BUTTON_PRIMARY_HOVER : colors::BUTTON_PRIMARY;

        resourceManager->get<gfx::IWindow>()->drawRoundedRectangleFilled(
            optionBgColor,
            {static_cast<size_t>(optionPos.getX()), static_cast<size_t>(optionPos.getY())},
            {static_cast<size_t>(optionSize.getX()), static_cast<size_t>(optionSize.getY())},
            5.0f
        );

        resourceManager->get<gfx::IWindow>()->drawRoundedRectangleOutline(
            colors::UI_DISABLED,
            {static_cast<size_t>(optionPos.getX()), static_cast<size_t>(optionPos.getY())},
            {static_cast<size_t>(optionSize.getX()), static_cast<size_t>(optionSize.getY())},
            5.0f
        );

        if (!(_options[i].empty())) {
            auto textSize = resourceManager->get<gfx::IWindow>()->getTextSize(
                _options[i], _fontPath, getFontSize()
            );
            float textX = optionPos.getX() + 10.0f;
            float textY = optionPos.getY() +
                (optionSize.getY() - static_cast<float>(textSize.second)) / 2.0f;

            resourceManager->get<gfx::IWindow>()->drawText(
                _options[i],
                colors::UI_TEXT,
                {static_cast<size_t>(textX), static_cast<size_t>(textY)},
                _fontPath,
                getFontSize()
            );
        }
    }
}

void Dropdown::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    if (!_visible) {
        return;
    }

    AFocusableElement::handleInput(mousePos, mousePressed);

    bool justPressed = mousePressed && !_dropdownWasPressed;
    _dropdownWasPressed = mousePressed;

    if (_isOpen) {
        _hoveredOptionIndex = -1;
        for (size_t i = 0; i < _options.size(); ++i) {
            if (isMouseOverOption(mousePos, i)) {
                _hoveredOptionIndex = static_cast<int>(i);
                break;
            }
        }

        if (justPressed) {
            if (_hoveredOptionIndex >= 0) {
                setSelectedIndex(static_cast<size_t>(_hoveredOptionIndex));
                close();
                return;
            }
            if (!containsPoint(mousePos)) {
                close();
            }
        }
    } else {
        _hoveredOptionIndex = -1;
        if (justPressed && containsPoint(mousePos)) {
            open();
        }
    }
}

void Dropdown::update(float deltaTime) {
    (void)deltaTime;
}

bool Dropdown::containsPoint(const math::Vector2f& point) const {
    if (!_visible) {
        return false;
    }

    if (UIElement::containsPoint(point)) {
        return true;
    }

    if (_isOpen) {
        for (size_t i = 0; i < _options.size(); ++i) {
            if (isMouseOverOption(point, i)) {
                return true;
            }
        }
    }

    return false;
}

void Dropdown::setOptions(const std::vector<std::string>& options) {
    _options = options;
    if (_selectedIndex >= _options.size()) {
        _selectedIndex = 0;
        _hasSelection = false;
    }
}

void Dropdown::addOption(const std::string& option) {
    _options.push_back(option);
}

void Dropdown::clearOptions() {
    _options.clear();
    _selectedIndex = 0;
    _hasSelection = false;
}

const std::vector<std::string>& Dropdown::getOptions() const {
    return _options;
}

const std::string& Dropdown::getSelectedOption() const {
    static const std::string emptyString = "";
    if (_hasSelection && _selectedIndex < _options.size()) {
        return _options[_selectedIndex];
    }
    return emptyString;
}

void Dropdown::setSelectedIndex(size_t index) {
    if (index < _options.size()) {
        _selectedIndex = index;
        _hasSelection = true;
        if (_onSelectionChanged) {
            _onSelectionChanged(_options[_selectedIndex], _selectedIndex);
        }
    }
}

size_t Dropdown::getSelectedIndex() const {
    return _selectedIndex;
}

const std::string& Dropdown::getSelectedValue() const {
    static const std::string emptyString = "";
    if (_hasSelection && _selectedIndex < _options.size()) {
        return _options[_selectedIndex];
    }
    return emptyString;
}

void Dropdown::setDirection(DropdownDirection direction) {
    _direction = direction;
}

DropdownDirection Dropdown::getDirection() const {
    return _direction;
}

void Dropdown::setPlaceholder(const std::string& placeholder) {
    _placeholder = placeholder;
}

const std::string& Dropdown::getPlaceholder() const {
    return _placeholder;
}

void Dropdown::setOnSelectionChanged(
    std::function<void(const std::string&, size_t)> callback
) {
    _onSelectionChanged = callback;
}

void Dropdown::open() {
    _isOpen = true;
}

void Dropdown::close() {
    _isOpen = false;
    _hoveredOptionIndex = -1;
}

bool Dropdown::isOpen() const {
    return _isOpen;
}

bool Dropdown::isMouseOverOption(const math::Vector2f& mousePos, size_t optionIndex) const {
    math::Vector2f optionPos = getOptionPosition(optionIndex);
    math::Vector2f optionSize = getAbsoluteSize();

    return mousePos.getX() >= optionPos.getX() &&
           mousePos.getX() <= optionPos.getX() + optionSize.getX() &&
           mousePos.getY() >= optionPos.getY() &&
           mousePos.getY() <= optionPos.getY() + optionSize.getY();
}

math::Vector2f Dropdown::getOptionPosition(size_t optionIndex) const {
    math::Vector2f basePos = getAbsolutePosition();
    math::Vector2f size = getAbsoluteSize();
    const float OPTION_SPACING = 5.0f;

    switch (_direction) {
        case DropdownDirection::Down:
            return {basePos.getX(), basePos.getY() + size.getY() +
                OPTION_SPACING + (size.getY() + OPTION_SPACING) * static_cast<float>(optionIndex)};
        case DropdownDirection::Up:
            return {basePos.getX(), basePos.getY() - size.getY() -
                OPTION_SPACING - (size.getY() + OPTION_SPACING) * static_cast<float>(optionIndex)};
        case DropdownDirection::Right:
            return {basePos.getX() + size.getX() + OPTION_SPACING +
                (size.getX() + OPTION_SPACING) * static_cast<float>(optionIndex), basePos.getY()};
        case DropdownDirection::Left:
            return {basePos.getX() - size.getX() - OPTION_SPACING -
                (size.getX() + OPTION_SPACING) * static_cast<float>(optionIndex), basePos.getY()};
        default:
            return basePos;
    }
}

math::Vector2f Dropdown::getDropdownSize() const {
    if (!_isOpen) return getAbsoluteSize();

    math::Vector2f size = getAbsoluteSize();
    size_t optionCount = _options.size();

    switch (_direction) {
        case DropdownDirection::Down:
        case DropdownDirection::Up:
            size.setY(size.getY() * static_cast<float>(optionCount + 1));
            break;
        case DropdownDirection::Left:
        case DropdownDirection::Right:
            size.setX(size.getX() * static_cast<float>(optionCount + 1));
            break;
    }

    return size;
}

void Dropdown::setTextColor(const gfx::color_t& color) {
    _textColor = color;
}

void Dropdown::setPlaceholderColor(const gfx::color_t& color) {
    _placeholderColor = color;
}

void Dropdown::setFontPath(const std::string& fontPath) {
    _fontPath = fontPath;
}

void Dropdown::setBaseFontSize(size_t fontSize) {
    _baseFontSize = fontSize;
}

size_t Dropdown::getBaseFontSize() const {
    return _baseFontSize;
}

size_t Dropdown::getFontSize() const {
    return static_cast<size_t>(static_cast<float>(_baseFontSize) * getScaleFactor());
}

gfx::color_t Dropdown::_getCurrentColor() const {
    switch (_state) {
        case UIState::Hovered:
            return _hoveredColor;
        case UIState::Pressed:
            return _pressedColor;
        case UIState::Disabled:
            return _disabledColor;
        case UIState::Focused:
            return _focusedColor;
        default:
            return _normalColor;
    }
}

void Dropdown::drawArrow(float arrowX, float arrowY, float arrowSize) {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return;
    }

    struct ArrowSegment {
        float offsetX;
        float offsetY;
        float width;
        float height;
    };

    std::vector<ArrowSegment> segments;

    if (_direction == DropdownDirection::Down) {
        segments = {{0, 0, arrowSize, 2}, {2, 2, arrowSize - 4, 2}, {4, 4, arrowSize - 8, 2}};
    } else if (_direction == DropdownDirection::Up) {
        segments = {{0, arrowSize - 2, arrowSize, 2}, {2, arrowSize - 4, arrowSize - 4, 2},
            {4, arrowSize - 6, arrowSize - 8, 2}};
    } else if (_direction == DropdownDirection::Right) {
        segments = {{0, 0, 2, arrowSize}, {2, 2, 2, arrowSize - 4}, {4, 4, 2, arrowSize - 8}};
    } else if (_direction == DropdownDirection::Left) {
        segments = {{arrowSize - 2, 0, 2, arrowSize}, {arrowSize - 4, 2, 2, arrowSize - 4},
            {arrowSize - 6, 4, 2, arrowSize - 8}};
    }

    for (const auto& segment : segments) {
        resourceManager->get<gfx::IWindow>()->drawFilledRectangle(
            _textColor,
            {static_cast<size_t>(arrowX + segment.offsetX),
                static_cast<size_t>(arrowY + segment.offsetY)},
            {static_cast<size_t>(segment.width), static_cast<size_t>(segment.height)}
        );
    }
}

}  // namespace ui
