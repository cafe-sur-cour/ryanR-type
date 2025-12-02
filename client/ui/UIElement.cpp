/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UIElement
*/

#include "UIElement.hpp"
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../common/constants.hpp"

namespace ui {

UIElement::UIElement(std::shared_ptr<ecs::ResourceManager> resourceManager)
    : _resourceManager(resourceManager) {
}

void UIElement::setPosition(const math::Vector2f& position) {
    _position = position;
}

void UIElement::setSize(const math::Vector2f& size) {
    _size = size;
}

math::Vector2f UIElement::getPosition() const {
    return _position;
}

math::Vector2f UIElement::getSize() const {
    return _size;
}

math::Vector2f UIElement::getAbsolutePosition() const {
    auto windowSize = getWindowSize();
    return math::Vector2f(
        _position.getX() * static_cast<float>(windowSize.first),
        _position.getY() * static_cast<float>(windowSize.second)
    );
}

math::Vector2f UIElement::getAbsoluteSize() const {
    auto windowSize = getWindowSize();
    return math::Vector2f(
        _size.getX() * static_cast<float>(windowSize.first),
        _size.getY() * static_cast<float>(windowSize.second)
    );
}

void UIElement::addChild(std::shared_ptr<UIElement> child) {
    if (child) {
        child->setParent(weak_from_this());
        _children.push_back(child);
    }
}

void UIElement::removeChild(std::shared_ptr<UIElement> child) {
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end()) {
        (*it)->setParent(std::weak_ptr<UIElement>());
        _children.erase(it);
    }
}

void UIElement::handleInput(const math::Vector2f& mousePos, bool mousePressed) {
    if (!_visible || _state == UIState::Disabled) {
        return;
    }

    bool wasHovered = (_state == UIState::Hovered);
    bool containsMouse = containsPoint(mousePos);

    if (containsMouse) {
        if (mousePressed) {
            if (_state != UIState::Pressed) {
                _state = UIState::Pressed;
                if (_onClick)
                    _onClick();
            }
        } else {
            if (_state == UIState::Pressed) {
                _state = UIState::Hovered;
                if (_onRelease)
                    _onRelease();
            } else {
                _state = UIState::Hovered;
                if (!wasHovered && _onHover)
                    _onHover();
            }
        }
    } else {
        _state = UIState::Normal;
    }

    for (auto it = _children.rbegin(); it != _children.rend(); ++it) {
        (*it)->handleInput(mousePos, mousePressed);
    }
}

bool UIElement::containsPoint(const math::Vector2f& point) const {
    if (!_visible) {
        return false;
    }

    math::Vector2f absPos = getAbsolutePosition();
    math::Vector2f absSize = getAbsoluteSize();

    return (point.getX() >= absPos.getX() &&
            point.getX() <= absPos.getX() + absSize.getX() &&
            point.getY() >= absPos.getY() &&
            point.getY() <= absPos.getY() + absSize.getY());
}

void UIElement::render() {
    if (!_visible) {
        return;
    }

    for (auto& child : _children) {
        child->render();
    }
}

void UIElement::update(float deltaTime) {
    for (auto& child : _children) {
        child->update(deltaTime);
    }
}

void UIElement::setVisible(bool visible) {
    _visible = visible;
}

bool UIElement::isVisible() const {
    return _visible;
}

void UIElement::setState(UIState state) {
    _state = state;
}

UIState UIElement::getState() const {
    return _state;
}

void UIElement::setParent(std::weak_ptr<UIElement> parent) {
    _parent = parent;
}

std::shared_ptr<UIElement> UIElement::getParent() const {
    return _parent.lock();
}

const std::vector<std::shared_ptr<UIElement>>& UIElement::getChildren() const {
    return _children;
}

void UIElement::setOnClick(std::function<void()> callback) {
    _onClick = callback;
}

void UIElement::setOnHover(std::function<void()> callback) {
    _onHover = callback;
}

void UIElement::setOnRelease(std::function<void()> callback) {
    _onRelease = callback;
}

std::pair<int, int> UIElement::getWindowSize() const {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return {constants::WINDOW_WIDTH, constants::WINDOW_HEIGHT};
    }
    return resourceManager->get<gfx::IWindow>()->getWindowSize();
}

}  // namespace ui
