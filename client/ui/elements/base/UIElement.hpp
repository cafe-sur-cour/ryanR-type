/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UIElement
*/

#ifndef UIELEMENT_HPP_
#define UIELEMENT_HPP_

#include <memory>
#include <vector>
#include <functional>
#include "../../../../common/types/Vector2f.hpp"
#include "../../../../common/resourceManager/ResourceManager.hpp"

namespace ui {

enum class UIState {
    Normal,
    Hovered,
    Pressed,
    Disabled,
    Focused
};

enum class UIScale {
    Small,
    Normal,
    Large
};

class UIElement : public std::enable_shared_from_this<UIElement> {
    public:
        UIElement(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~UIElement() = default;

        void setPosition(const math::Vector2f& position);
        void setSize(const math::Vector2f& size);
        math::Vector2f getPosition() const;
        math::Vector2f getSize() const;

        math::Vector2f getAbsolutePosition() const;
        math::Vector2f getAbsoluteSize() const;

        void setVisible(bool visible);
        bool isVisible() const;

        void setScalingEnabled(bool enabled);
        bool isScalingEnabled() const;

        void setFocusEnabled(bool enabled);
        bool isFocusEnabled() const;

        void setState(UIState state);
        UIState getState() const;

        virtual void setScale(UIScale scale);
        UIScale getScale() const;

        void setParent(std::weak_ptr<UIElement> parent);
        std::shared_ptr<UIElement> getParent() const;
        void addChild(std::shared_ptr<UIElement> child);
        void removeChild(std::shared_ptr<UIElement> child);
        const std::vector<std::shared_ptr<UIElement>>& getChildren() const;

        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed);
        virtual bool containsPoint(const math::Vector2f& point) const;

        void setOnClick(std::function<void()> callback);
        void setOnHover(std::function<void()> callback);
        void setOnRelease(std::function<void()> callback);

        virtual void render();

        virtual void update(float deltaTime);

    protected:
        std::weak_ptr<ResourceManager> _resourceManager;
        math::Vector2f _position;
        math::Vector2f _size;
        bool _visible = true;
        bool _scalingEnabled = true;
        bool _focusEnabled = true;
        UIState _state = UIState::Normal;
        UIScale _scale = UIScale::Normal;
        std::weak_ptr<UIElement> _parent;
        std::vector<std::shared_ptr<UIElement>> _children;

        bool _pressedInside = false;
        bool _wasPressed = false;

        std::function<void()> _onClick;
        std::function<void()> _onHover;
        std::function<void()> _onRelease;

        std::pair<int, int> getWindowSize() const;
        std::pair<int, int> getLogicalSize() const;

        float getScaleFactor() const;
};

} // namespace ui

#endif /* !UIELEMENT_HPP_ */
