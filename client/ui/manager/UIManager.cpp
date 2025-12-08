/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UIManager
*/

#include "UIManager.hpp"
#include <algorithm>
#include <cmath>
#include <memory>

namespace ui {

UIManager::UIManager()
    : _navigationManager(std::make_shared<UINavigationManager>()),
    _lastMousePos(0.0f, 0.0f),
    _mouseMovementDetected(false) {
    _lastMousePos = math::Vector2f(
        constants::INVALID_MOUSE_POSITION,
        constants::INVALID_MOUSE_POSITION);
}

void UIManager::addElement(std::shared_ptr<UIElement> element) {
    if (!element)
        return;

    auto it = std::find(_elements.begin(), _elements.end(), element);
    if (it == _elements.end()) {
        element->setScale(_globalScale);
        _elements.push_back(element);
        if (auto focusable = std::dynamic_pointer_cast<IFocusable>(element)) {
            _navigationManager->addFocusableElement(focusable);
        }

        refreshNavigationElements();
    }
}

void UIManager::removeElement(std::shared_ptr<UIElement> element) {
    if (!element) {
        return;
    }

    auto it = std::find(_elements.begin(), _elements.end(), element);
    if (it != _elements.end()) {
        _elements.erase(it);

        if (auto focusable = std::dynamic_pointer_cast<IFocusable>(element)) {
            _navigationManager->removeFocusableElement(focusable);
        }
    }
}

void UIManager::clearElements() {
    _elements.clear();
    _navigationManager->clearFocusableElements();
}

void UIManager::update(float deltaTime) {
    _elements.erase(
        std::remove_if(_elements.begin(), _elements.end(),
            [](const std::shared_ptr<UIElement>& element) {
                return !element;
            }),
        _elements.end());

    for (auto& element : _elements) {
        if (element) {
            element->update(deltaTime);
        }
    }
}

void UIManager::render() {
    for (auto& element : _elements) {
        if (element) {
            element->render();
        }
    }
}

void UIManager::handleMouseInput(const math::Vector2f& mousePos, bool mousePressed) {
    if (_lastMousePos.getX() > constants::INVALID_MOUSE_POSITION && hasMouseMoved(mousePos)) {
        _navigationManager->onMouseMovement();
        _mouseMovementDetected = true;
    }
    _lastMousePos = mousePos;

    for (auto& element : _elements) {
        if (element && element->isVisible()) {
            element->handleInput(mousePos, mousePressed);
        }
    }
}

bool UIManager::handleNavigationInput(ecs::InputAction action) {
    return _navigationManager->handleNavigationInput(action);
}

bool UIManager::handleNavigationInputs(
    std::shared_ptr<ecs::IInputProvider> inputProvider,
    float deltaTime
) {
    if (!inputProvider)
        return false;

    if (_navigationCooldown > 0.0f)
        _navigationCooldown -= deltaTime;

    if (_navigationCooldown > 0.0f)
        return false;

    bool navigationTriggered = false;

    if (inputProvider->isActionPressed(ecs::InputAction::MENU_UP)) {
        handleNavigationInput(ecs::InputAction::MENU_UP);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_DOWN)) {
        handleNavigationInput(ecs::InputAction::MENU_DOWN);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_LEFT)) {
        handleNavigationInput(ecs::InputAction::MENU_LEFT);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_RIGHT)) {
        handleNavigationInput(ecs::InputAction::MENU_RIGHT);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_SELECT)) {
        handleNavigationInput(ecs::InputAction::MENU_SELECT);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
        handleNavigationInput(ecs::InputAction::MENU_BACK);
        navigationTriggered = true;
    }

    if (navigationTriggered)
        _navigationCooldown = constants::NAVIGATION_COOLDOWN_TIME;

    return navigationTriggered;
}

std::shared_ptr<UINavigationManager> UIManager::getNavigationManager() {
    return _navigationManager;
}

void UIManager::setNavigationEnabled(bool enabled) {
    _navigationManager->setNavigationEnabled(enabled);
}

bool UIManager::isNavigationEnabled() const {
    return _navigationManager->isNavigationEnabled();
}

bool UIManager::focusFirstElement() {
    return _navigationManager->focusFirstElement();
}

void UIManager::clearFocus() {
    _navigationManager->clearFocus();
}

std::shared_ptr<IFocusable> UIManager::getFocusedElement() const {
    return _navigationManager->getFocusedElement();
}

bool UIManager::hasMouseMoved(const math::Vector2f& mousePos) {
    const float threshold = 20.0f;
    float deltaX = mousePos.getX() - _lastMousePos.getX();
    float deltaY = mousePos.getY() - _lastMousePos.getY();
    float distance = std::sqrt(deltaX * deltaX + deltaY * deltaY);

    return distance > threshold;
}

void UIManager::refreshNavigationElements() {
    _navigationManager->clearFocusableElements();

    for (auto& element : _elements) {
        if (auto focusable = std::dynamic_pointer_cast<IFocusable>(element)) {
            if (focusable->canBeFocused()) {
                _navigationManager->addFocusableElement(focusable);
            }
        }
    }
}

void UIManager::setGlobalScale(UIScale scale) {
    _globalScale = scale;
    for (auto& element : _elements) {
        if (element) {
            element->setScale(scale);
        }
    }
}

void UIManager::cycleGlobalScale() {
    UIScale nextScale;
    switch (_globalScale) {
        case UIScale::Normal:
            nextScale = UIScale::Large;
            break;
        case UIScale::Large:
            nextScale = UIScale::Small;
            break;
        case UIScale::Small:
            nextScale = UIScale::Normal;
            break;
        default:
            nextScale = UIScale::Normal;
            break;
    }
    setGlobalScale(nextScale);
}

UIScale UIManager::getGlobalScale() const {
    return _globalScale;
}

}  // namespace ui
