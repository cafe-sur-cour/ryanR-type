/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AFocusableElement - Abstract base class for focusable UI elements
*/

#include "AFocusableElement.hpp"

namespace ui {

AFocusableElement::AFocusableElement(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager), _focused(false) {
}

void AFocusableElement::setFocused(bool focused) {
    if (_focused != focused) {
        _focused = focused;
        onFocusStateChanged(focused);

        if (focused) {
            setState(UIState::Focused);
            onFocusGained();
        } else {
            setState(UIState::Normal);
            onFocusLost();
        }
    }
}

bool AFocusableElement::isFocused() const {
    return _focused;
}

bool AFocusableElement::canBeFocused() const {
    return isVisible() && getState() != UIState::Disabled;
}

void AFocusableElement::onFocusGained() {
    if (_onFocusGained) {
        _onFocusGained();
    }
}

void AFocusableElement::onFocusLost() {
    if (_onFocusLost) {
        _onFocusLost();
    }
}

void AFocusableElement::onActivated() {
    if (_onActivated) {
        _onActivated();
    }
}

void AFocusableElement::setOnFocusGained(std::function<void()> callback) {
    _onFocusGained = callback;
}

void AFocusableElement::setOnFocusLost(std::function<void()> callback) {
    _onFocusLost = callback;
}

void AFocusableElement::setOnActivated(std::function<void()> callback) {
    _onActivated = callback;
}

void AFocusableElement::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    if (!_visible || _state == UIState::Disabled)
        return;

    bool wasHovered = (_state == UIState::Hovered);
    bool containsMouse = containsPoint(mousePos);

    if (containsMouse) {
        if (mousePressed) {
            if (_state != UIState::Pressed) {
                setState(UIState::Pressed);
                if (_onClick)
                    _onClick();
            }
        } else {
            if (_state == UIState::Pressed) {
                if (!_focused) {
                    setState(UIState::Hovered);
                }
                if (_onRelease)
                    _onRelease();
            } else {
                if (!_focused) {
                    setState(UIState::Hovered);
                    if (!wasHovered && _onHover)
                        _onHover();
                }
            }
        }
    } else {
        if (!_focused) {
            setState(UIState::Normal);
        }
    }

    for (auto& child : getChildren()) {
        child->handleInput(mousePos, mousePressed);
    }
}

void AFocusableElement::onFocusStateChanged(bool focused) {
    (void)focused;
}

}  // namespace ui
