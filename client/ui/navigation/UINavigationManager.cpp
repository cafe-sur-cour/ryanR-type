/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UINavigationManager
*/

#include "UINavigationManager.hpp"
#include <algorithm>
#include <memory>

namespace ui {

UINavigationManager::UINavigationManager()
    : _navigationEnabled(true), _focusDisabled(false), _onFocusChanged(nullptr) {
}

void UINavigationManager::addFocusableElement(std::shared_ptr<IFocusable> element) {
    if (!element)
        return;

    auto it = std::find_if(_focusableElements.begin(), _focusableElements.end(),
        [element](const std::weak_ptr<IFocusable>& weak) {
            auto shared = weak.lock();
            return shared && shared.get() == element.get();
        });

    if (it == _focusableElements.end()) {
        _focusableElements.push_back(element);
    }
}

void UINavigationManager::removeFocusableElement(std::shared_ptr<IFocusable> element) {
    if (!element)
        return;

    auto currentFocused = _currentFocused.lock();
    if (currentFocused && currentFocused.get() == element.get()) {
        clearFocus();
    }

    _focusableElements.erase(
        std::remove_if(_focusableElements.begin(), _focusableElements.end(),
            [element](const std::weak_ptr<IFocusable>& weak) {
                auto shared = weak.lock();
                return !shared || shared.get() == element.get();
            }),
        _focusableElements.end());
}

void UINavigationManager::clearFocusableElements() {
    clearFocus();
    _focusableElements.clear();
}

bool UINavigationManager::handleNavigationInput(ecs::InputAction action) {
    if (!_navigationEnabled)
        return false;

    enableFocus();

    switch (action) {
        case ecs::InputAction::MENU_UP:
            return navigateInDirection(NavigationDirection::Up);
        case ecs::InputAction::MENU_DOWN:
            return navigateInDirection(NavigationDirection::Down);
        case ecs::InputAction::MENU_LEFT:
            {
                auto focused = _currentFocused.lock();
                if (focused && focused->onNavigateLeft()) {
                    return true;
                }
                return navigateInDirection(NavigationDirection::Left);
            }
        case ecs::InputAction::MENU_RIGHT:
            {
                auto focused = _currentFocused.lock();
                if (focused && focused->onNavigateRight()) {
                    return true;
                }
                return navigateInDirection(NavigationDirection::Right);
            }
        case ecs::InputAction::MENU_SELECT:
            {
                auto focused = _currentFocused.lock();
                if (focused) {
                    focused->onActivated();
                    return true;
                }
                return false;
            }
        default:
            return false;
    }
}

bool UINavigationManager::setFocus(std::shared_ptr<IFocusable> element) {
    if (!element || !element->canBeFocused() || _focusDisabled)
        return false;

    auto currentFocused = _currentFocused.lock();
    if (currentFocused) {
        currentFocused->setFocused(false);
        currentFocused->onFocusLost();
    }

    _currentFocused = element;
    element->setFocused(true);
    element->onFocusGained();

    if (_onFocusChanged) {
        _onFocusChanged(element);
    }

    return true;
}

std::shared_ptr<IFocusable> UINavigationManager::getFocusedElement() const {
    return _currentFocused.lock();
}

void UINavigationManager::clearFocus() {
    auto currentFocused = _currentFocused.lock();
    if (currentFocused) {
        currentFocused->setFocused(false);
        currentFocused->onFocusLost();
    }
    _currentFocused.reset();

    if (_onFocusChanged) {
        _onFocusChanged(nullptr);
    }
}

bool UINavigationManager::focusFirstElement() {
    cleanupExpiredElements();

    for (auto& weakElement : _focusableElements) {
        auto element = weakElement.lock();
        if (element && element->canBeFocused()) {
            return setFocus(element);
        }
    }
    return false;
}

bool UINavigationManager::focusNextElement() {
    cleanupExpiredElements();

    if (_focusableElements.empty())
        return false;

    int currentIndex = getCurrentFocusedIndex();
    size_t startIndex = (currentIndex >= 0) ? static_cast<size_t>(currentIndex + 1) : 0;

    for (size_t i = startIndex; i < _focusableElements.size(); ++i) {
        auto element = _focusableElements[i].lock();
        if (element && element->canBeFocused()) {
            return setFocus(element);
        }
    }

    for (size_t i = 0; i < static_cast<size_t>(currentIndex); ++i) {
        auto element = _focusableElements[i].lock();
        if (element && element->canBeFocused()) {
            return setFocus(element);
        }
    }

    return false;
}

bool UINavigationManager::focusPreviousElement() {
    cleanupExpiredElements();

    if (_focusableElements.empty())
        return false;

    int currentIndex = getCurrentFocusedIndex();
    int startIndex = (currentIndex > 0) ?
        currentIndex - 1 : static_cast<int>(_focusableElements.size()) - 1;

    for (int i = startIndex; i >= 0; --i) {
        auto element = _focusableElements[static_cast<size_t>(i)].lock();
        if (element && element->canBeFocused()) {
            return setFocus(element);
        }
    }

    for (int i = static_cast<int>(_focusableElements.size()) - 1; i > currentIndex; --i) {
        auto element = _focusableElements[static_cast<size_t>(i)].lock();
        if (element && element->canBeFocused()) {
            return setFocus(element);
        }
    }

    return false;
}

void UINavigationManager::setNavigationEnabled(bool enabled) {
    _navigationEnabled = enabled;
    if (!enabled) {
        clearFocus();
    }
}

bool UINavigationManager::isNavigationEnabled() const {
    return _navigationEnabled;
}

void UINavigationManager::setOnFocusChanged(
    std::function<void(std::shared_ptr<IFocusable>)> callback
) {
    _onFocusChanged = callback;
}

void UINavigationManager::onMouseMovement() {
    _focusDisabled = true;
    clearFocus();
}

void UINavigationManager::enableFocus() {
    _focusDisabled = false;
}

bool UINavigationManager::isFocusDisabled() const {
    return _focusDisabled;
}

void UINavigationManager::cleanupExpiredElements() {
    _focusableElements.erase(
        std::remove_if(_focusableElements.begin(), _focusableElements.end(),
            [](const std::weak_ptr<IFocusable>& weak) {
                return weak.expired();
            }),
        _focusableElements.end());
}

int UINavigationManager::getCurrentFocusedIndex() const {
    auto currentFocused = _currentFocused.lock();
    if (!currentFocused) {
        return -1;
    }

    for (size_t i = 0; i < _focusableElements.size(); ++i) {
        auto element = _focusableElements[i].lock();
        if (element && element.get() == currentFocused.get()) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

bool UINavigationManager::navigateInDirection(NavigationDirection direction) {
    switch (direction) {
        case NavigationDirection::Up:
            return focusPreviousElement();
        case NavigationDirection::Down:
            return focusNextElement();
        case NavigationDirection::Left:
            return focusPreviousElement();
        case NavigationDirection::Right:
            return focusNextElement();
        default:
            return false;
    }
}

}  // namespace ui
