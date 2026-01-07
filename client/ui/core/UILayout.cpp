/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UILayout
*/

#include "UILayout.hpp"
#include <algorithm>
#include <memory>

namespace ui {

UILayout::UILayout(
    std::shared_ptr<ResourceManager> resourceManager,
    const LayoutConfig& config)
    : UIElement(resourceManager), _config(config) {
}

void UILayout::addElement(std::shared_ptr<UIElement> element) {
    if (!element) {
        return;
    }

    auto it = std::find(_layoutElements.begin(), _layoutElements.end(), element);
    if (it == _layoutElements.end()) {
        element->setScale(_scale);
        _layoutElements.push_back(element);
        addChild(element);
        updateLayout();
    }
}

void UILayout::removeElement(std::shared_ptr<UIElement> element) {
    if (!element) {
        return;
    }

    auto it = std::find(_layoutElements.begin(), _layoutElements.end(), element);
    if (it != _layoutElements.end()) {
        _layoutElements.erase(it);
        removeChild(element);
        updateLayout();
    }
}

void UILayout::clearElements() {
    for (auto& element : _layoutElements) {
        removeChild(element);
    }
    _layoutElements.clear();
    updateLayout();
}

void UILayout::setDirection(LayoutDirection direction) {
    _config.direction = direction;
    updateLayout();
}

void UILayout::setAlignment(LayoutAlignment alignment) {
    _config.alignment = alignment;
    updateLayout();
}

void UILayout::setSpacing(float spacing) {
    _config.spacing = spacing;
    updateLayout();
}

void UILayout::setPadding(const math::Vector2f& padding) {
    _config.padding = padding;
    updateLayout();
}

void UILayout::setOffset(const math::Vector2f& offset) {
    _config.offset = offset;
    applyAnchor();
}

void UILayout::setAutoResize(bool autoResize) {
    _config.autoResize = autoResize;
    updateLayout();
}

void UILayout::setAnchor(AnchorX anchorX, AnchorY anchorY) {
    _config.anchorX = anchorX;
    _config.anchorY = anchorY;
    applyAnchor();
}

void UILayout::setBackgroundEnabled(bool enabled) {
    _config.background.enabled = enabled;
}

void UILayout::setBackgroundFillColor(const color_t& color) {
    _config.background.fillColor = color;
}

void UILayout::setBackgroundOutlineColor(const color_t& color) {
    _config.background.outlineColor = color;
}

void UILayout::setBackgroundCornerRadius(float radius) {
    _config.background.cornerRadius = radius;
}

LayoutDirection UILayout::getDirection() const {
    return _config.direction;
}

LayoutAlignment UILayout::getAlignment() const {
    return _config.alignment;
}

float UILayout::getSpacing() const {
    return _config.spacing;
}

math::Vector2f UILayout::getPadding() const {
    return _config.padding;
}

bool UILayout::isAutoResize() const {
    return _config.autoResize;
}

float UILayout::getScaledSpacing() const {
    return _config.spacing * getScaleFactor();
}

void UILayout::updateLayout() {
    if (_layoutElements.empty()) {
        return;
    }

    calculatePositions();

    if (_config.autoResize) {
        float totalSize = getTotalSize();
        if (_config.direction == LayoutDirection::Horizontal) {
            _size = math::Vector2f(
                totalSize + _config.padding.getX() * 2,
                _size.getY()
            );
        } else {
            _size = math::Vector2f(
                _size.getX(),
                totalSize + _config.padding.getY() * 2
            );
        }
    }

    applyAnchor();
}

void UILayout::setScale(UIScale scale) {
    UIElement::setScale(scale);

    for (auto& element : _layoutElements) {
        if (element) {
            element->setScale(scale);
        }
    }
    updateLayout();
}

void UILayout::applyAnchor() {
    if (_config.anchorX == AnchorX::None && _config.anchorY == AnchorY::None) {
        return;
    }

    auto logicalSize = getLogicalSize();
    float windowWidth = static_cast<float>(logicalSize.first);
    float windowHeight = static_cast<float>(logicalSize.second);

    math::Vector2f anchoredPos(0.0f, 0.0f);
    float scaledPaddingX = _config.padding.getX() * getScaleFactor();
    float scaledPaddingY = _config.padding.getY() * getScaleFactor();
    float actualWidth = 0.0f;
    float actualHeight = 0.0f;

    if (_config.direction == LayoutDirection::Horizontal) {
        float totalSize = getTotalSize();
        actualWidth = totalSize + 2.0f * scaledPaddingX;
        for (auto& element : _layoutElements) {
            if (element && element->isVisible()) {
                float elemHeight = element->getAbsoluteSize().getY();
                if (elemHeight > actualHeight) {
                    actualHeight = elemHeight;
                }
            }
        }
        actualHeight += 2.0f * scaledPaddingY;
    } else {
        float totalSize = getTotalSize();
        actualHeight = totalSize + 2.0f * scaledPaddingY;
        for (auto& element : _layoutElements) {
            if (element && element->isVisible()) {
                float elemWidth = element->getAbsoluteSize().getX();
                if (elemWidth > actualWidth) {
                    actualWidth = elemWidth;
                }
            }
        }
        actualWidth += 2.0f * scaledPaddingX;
    }

    float posX = 0.0f;
    switch (_config.anchorX) {
        case AnchorX::Left:
            posX = _config.offset.getX();
            break;
        case AnchorX::Center:
            posX = (windowWidth - actualWidth) / 2.0f + _config.offset.getX();
            break;
        case AnchorX::Right:
            posX = windowWidth - actualWidth + _config.offset.getX();
            break;
        case AnchorX::None:
            posX = 0.0f;
            break;
    }

    float posY = 0.0f;
    switch (_config.anchorY) {
        case AnchorY::Top:
            posY = _config.offset.getY();
            break;
        case AnchorY::Center:
            posY = (windowHeight - actualHeight) / 2.0f + _config.offset.getY();
            break;
        case AnchorY::Bottom:
            posY = windowHeight - actualHeight + _config.offset.getY();
            break;
        case AnchorY::None:
            posY = 0.0f;
            break;
    }

    anchoredPos = math::Vector2f(posX, posY);
    _position = anchoredPos;
}

float UILayout::getTotalSize() const {
    float total = 0.0f;
    float scaledSpacing = getScaledSpacing();

    for (size_t i = 0; i < _layoutElements.size(); ++i) {
        auto& element = _layoutElements[i];
        if (!element || !element->isVisible()) {
            continue;
        }

        if (_config.direction == LayoutDirection::Horizontal) {
            total += element->getAbsoluteSize().getX();
        } else {
            total += element->getAbsoluteSize().getY();
        }

        if (i < _layoutElements.size() - 1) {
            total += scaledSpacing;
        }
    }

    return total;
}

void UILayout::calculatePositions() {
    if (_layoutElements.empty()) {
        return;
    }

    float currentOffset = 0.0f;
    float scaledSpacing = getScaledSpacing();
    float scaledPaddingX = _config.padding.getX() * getScaleFactor();
    float scaledPaddingY = _config.padding.getY() * getScaleFactor();

    float maxCrossSize = 0.0f;
    for (auto& element : _layoutElements) {
        if (element && element->isVisible()) {
            if (_config.direction == LayoutDirection::Horizontal) {
                float elemHeight = element->getAbsoluteSize().getY();
                if (elemHeight > maxCrossSize) maxCrossSize = elemHeight;
            } else {
                float elemWidth = element->getAbsoluteSize().getX();
                if (elemWidth > maxCrossSize) maxCrossSize = elemWidth;
            }
        }
    }

    for (size_t i = 0; i < _layoutElements.size(); ++i) {
        auto& element = _layoutElements[i];
        if (!element || !element->isVisible()) {
            continue;
        }

        math::Vector2f elementPos = calculateElementPosition(i, maxCrossSize);

        if (_config.direction == LayoutDirection::Horizontal) {
            elementPos = math::Vector2f(
                scaledPaddingX + currentOffset,
                elementPos.getY()
            );
            currentOffset += element->getAbsoluteSize().getX() + scaledSpacing;
        } else {
            elementPos = math::Vector2f(
                elementPos.getX(),
                scaledPaddingY + currentOffset
            );
            currentOffset += element->getAbsoluteSize().getY() + scaledSpacing;
        }

        element->setPosition(elementPos);
    }
}

math::Vector2f UILayout::calculateElementPosition(size_t index, float maxCrossSize) const {
    auto& element = _layoutElements[index];
    math::Vector2f position(0.0f, 0.0f);
    float scaledPaddingX = _config.padding.getX() * getScaleFactor();
    float scaledPaddingY = _config.padding.getY() * getScaleFactor();

    if (_config.direction == LayoutDirection::Horizontal) {
        switch (_config.alignment) {
            case LayoutAlignment::Start:
                position = math::Vector2f(0.0f, scaledPaddingY);
                break;
            case LayoutAlignment::Center:
                position = math::Vector2f(
                    0.0f,
                    (maxCrossSize - element->getAbsoluteSize().getY()) / 2.0f
                );
                break;
            case LayoutAlignment::End:
                position = math::Vector2f(
                    0.0f,
                    maxCrossSize - element->getAbsoluteSize().getY() - scaledPaddingY
                );
                break;
        }
    } else {
        switch (_config.alignment) {
            case LayoutAlignment::Start:
                position = math::Vector2f(scaledPaddingX, 0.0f);
                break;
            case LayoutAlignment::Center:
                position = math::Vector2f(
                    (maxCrossSize - element->getAbsoluteSize().getX()) / 2.0f,
                    0.0f
                );
                break;
            case LayoutAlignment::End:
                position = math::Vector2f(
                    maxCrossSize - element->getAbsoluteSize().getX() - scaledPaddingX,
                    0.0f
                );
                break;
        }
    }

    return position;
}

void UILayout::render() {
    if (!_visible) {
        return;
    }

    if (_config.background.enabled) {
        auto window = _resourceManager.lock()->get<IWindow>();
        math::Vector2f absSize = getAbsoluteSize();
        math::Vector2f absPos = getAbsolutePosition();

        if (_config.background.fillColor.a > 0) {
            window->drawRoundedRectangleFilled(
                _config.background.fillColor,
                {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
                {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
                _config.background.cornerRadius
            );
        }

        if (_config.background.outlineColor.a > 0) {
            window->drawRoundedRectangleOutline(
                _config.background.outlineColor,
                {static_cast<size_t>(absPos.getX()), static_cast<size_t>(absPos.getY())},
                {static_cast<size_t>(absSize.getX()), static_cast<size_t>(absSize.getY())},
                _config.background.cornerRadius
            );
        }
    }

    UIElement::render();
}

void UILayout::update(float deltaTime) {
    UIElement::update(deltaTime);
}

}  // namespace ui
