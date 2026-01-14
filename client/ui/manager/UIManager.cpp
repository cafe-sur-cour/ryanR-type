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
#include <functional>
#include <string>
#include "../elements/focusable/TextInput.hpp"
#include "../../../common/interfaces/IEvent.hpp"

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
        if (auto textInput = std::dynamic_pointer_cast<TextInput>(element)) {
            textInput->setOnNavigate([this](bool up) {
                if (up) {
                    _navigationManager->handleNavigationInput(ecs::InputAction::MENU_UP);
                } else {
                    _navigationManager->handleNavigationInput(ecs::InputAction::MENU_DOWN);
                }
            });
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
        auto focusedElement = getFocusedElement();
        if (!focusedElement || !std::dynamic_pointer_cast<TextInput>(focusedElement)) {
            _navigationManager->onMouseMovement();
            _mouseMovementDetected = true;
        }
    }
    _lastMousePos = mousePos;

    for (auto& element : _elements) {
        if (element && element->isVisible()) {
            element->handleInput(mousePos, mousePressed);
        }
    }

    if (_cursorCallback) {
        bool isHovering = isMouseHoveringAnyElement(mousePos);
        _cursorCallback(isHovering);
    }

    if (mousePressed) {
        bool clickedOnFocusable = false;
        for (auto& element : _elements) {
            if (element && element->isVisible() && element->containsPoint(mousePos)) {
                auto focusable = std::dynamic_pointer_cast<IFocusable>(element);
                if (focusable && focusable->canBeFocused()) {
                    clickedOnFocusable = true;
                    break;
                }
            }
        }
        if (!clickedOnFocusable) {
            _navigationManager->clearFocus();
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

    if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
        if (_onBack) {
            _onBack();
        }
        return true;
    }

    bool navigationTriggered = false;

    auto focusedElement = getFocusedElement();

    if (inputProvider->isActionPressed(ecs::InputAction::MENU_UP)) {
        if (_blockedActions.find(ecs::InputAction::MENU_UP) == _blockedActions.end()) {
            handleNavigationInput(ecs::InputAction::MENU_UP);
            navigationTriggered = true;
        }
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_DOWN)) {
        if (_blockedActions.find(ecs::InputAction::MENU_DOWN) == _blockedActions.end()) {
            handleNavigationInput(ecs::InputAction::MENU_DOWN);
            navigationTriggered = true;
        }
    }
    if (
        inputProvider->isActionPressed(ecs::InputAction::MENU_LEFT) &&
        !(focusedElement && std::dynamic_pointer_cast<TextInput>(focusedElement))
    ) {
        handleNavigationInput(ecs::InputAction::MENU_LEFT);
        navigationTriggered = true;
    }
    if (
        inputProvider->isActionPressed(ecs::InputAction::MENU_RIGHT) &&
        !(focusedElement && std::dynamic_pointer_cast<TextInput>(focusedElement))
    ) {
        handleNavigationInput(ecs::InputAction::MENU_RIGHT);
        navigationTriggered = true;
    }
    if (inputProvider->isActionPressed(ecs::InputAction::MENU_SELECT)) {
        handleNavigationInput(ecs::InputAction::MENU_SELECT);
        navigationTriggered = true;
    }

    if (navigationTriggered)
        _navigationCooldown = constants::NAVIGATION_COOLDOWN_TIME;

    _consumedTextKeys.clear();
    _blockedActions.clear();
    return navigationTriggered;
}

void UIManager::handleKeyboardInput(gfx::EventType event) {
    auto focusedElement = getFocusedElement();
    if (focusedElement) {
        if (auto textInput = std::dynamic_pointer_cast<TextInput>(focusedElement)) {
            if (event == gfx::EventType::V && _resourceManager) {
                bool ctrlPressed = _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::LCTRL) ||
                                   _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::RCTRL);
                if (ctrlPressed) {
                    std::string clipboardText =
                        _resourceManager->get<gfx::IWindow>()->getClipboardText();
                    if (!clipboardText.empty()) {
                        size_t start = clipboardText.find_first_not_of(" \t\n\r\f\v");
                        if (start != std::string::npos) {
                            size_t end = clipboardText.find_last_not_of(" \t\n\r\f\v");
                            std::string trimmedText =
                            clipboardText.substr(start, end - start + 1);
                            if (!trimmedText.empty()) {
                                textInput->handleTextInput(trimmedText);
                            }
                        }
                    }
                    return;
                }
            }
            if (event == gfx::EventType::C && _resourceManager) {
                bool ctrlPressed = _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::LCTRL) ||
                                   _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::RCTRL);
                if (ctrlPressed) {
                    std::string textToCopy = textInput->getText();
                    if (!textToCopy.empty()) {
                        _resourceManager->get<gfx::IWindow>()->setClipboardText(textToCopy);
                    }
                    return;
                }
            }
            if (event == gfx::EventType::BACKSPACE && _resourceManager) {
                bool ctrlPressed = _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::LCTRL) ||
                                   _resourceManager->get<gfx::IEvent>()->isKeyPressed(
                                    gfx::EventType::RCTRL);
                if (ctrlPressed) {
                    textInput->setText("");
                    return;
                }
            }

            bool isLetterKey = (event >= gfx::EventType::A && event <= gfx::EventType::Z);
            if (isLetterKey) {
                _consumedTextKeys.insert(event);
                if (event == gfx::EventType::Z) {
                    _blockedActions.insert(ecs::InputAction::MENU_UP);
                } else if (event == gfx::EventType::S) {
                    _blockedActions.insert(ecs::InputAction::MENU_DOWN);
                } else if (event == gfx::EventType::Q) {
                    _blockedActions.insert(ecs::InputAction::MENU_LEFT);
                } else if (event == gfx::EventType::D) {
                    _blockedActions.insert(ecs::InputAction::MENU_RIGHT);
                }
            }
            textInput->handleKeyboardInput(event);
        }
    }
}

void UIManager::handleTextInput(const std::string& text) {
    auto focusedElement = getFocusedElement();
    if (focusedElement) {
        if (auto textInput = std::dynamic_pointer_cast<TextInput>(focusedElement)) {
            textInput->handleTextInput(text);
        }
    }
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

    std::function<void(const std::shared_ptr<UIElement>&)> collectFocusables =
        [this, &collectFocusables](const std::shared_ptr<UIElement>& element) {
            if (!element) return;

            if (auto focusable = std::dynamic_pointer_cast<IFocusable>(element)) {
                if (focusable->canBeFocused()) {
                    _navigationManager->addFocusableElement(focusable);
                }
            }

            const auto& children = element->getChildren();
            for (const auto& child : children) {
                collectFocusables(child);
            }
        };

    for (auto& element : _elements) {
        collectFocusables(element);
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

void UIManager::setOnBack(std::function<void()> callback) {
    _onBack = callback;
}

void UIManager::setCursorCallback(std::function<void(bool)> callback) {
    _cursorCallback = callback;
}

void UIManager::setResourceManager(std::shared_ptr<ResourceManager> resourceManager) {
    _resourceManager = resourceManager;
}

bool UIManager::isMouseHoveringAnyElement(const math::Vector2f& mousePos) const {
    std::function<bool(const std::shared_ptr<UIElement>&)> checkElementAndChildren =
        [&checkElementAndChildren, &mousePos]
            (const std::shared_ptr<UIElement>& element) -> bool {
            if (!element || !element->isVisible()) {
                return false;
            }

            if (element->containsPoint(mousePos)) {
                if (auto focusable = std::dynamic_pointer_cast<IFocusable>(element)) {
                    if (focusable->canBeFocused()) {
                        return true;
                    }
                }
            }

            const auto& children = element->getChildren();
            for (const auto& child : children) {
                if (checkElementAndChildren(child)) {
                    return true;
                }
            }

            return false;
        };

    for (const auto& element : _elements) {
        if (checkElementAndChildren(element)) {
            return true;
        }
    }
    return false;
}

}  // namespace ui
