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
#include "../../common/types/Vector2f.hpp"
#include "../../common/ECS/resourceManager/ResourceManager.hpp"

namespace ui {

enum class UIState {
    Normal,
    Hovered,
    Pressed,
    Disabled
};

class UIElement : public std::enable_shared_from_this<UIElement> {
    public:
        UIElement(std::shared_ptr<ecs::ResourceManager> resourceManager);
        virtual ~UIElement() = default;

        void setPosition(const math::Vector2f& position);
        void setSize(const math::Vector2f& size);
        math::Vector2f getPosition() const { return _position; }
        math::Vector2f getSize() const { return _size; }

        math::Vector2f getAbsolutePosition() const;
        math::Vector2f getAbsoluteSize() const;

        void setVisible(bool visible) { _visible = visible; }
        bool isVisible() const { return _visible; }

        void setState(UIState state) { _state = state; }
        UIState getState() const { return _state; }

        void setParent(std::weak_ptr<UIElement> parent) { _parent = parent; }
        std::shared_ptr<UIElement> getParent() const { return _parent.lock(); }
        void addChild(std::shared_ptr<UIElement> child);
        void removeChild(std::shared_ptr<UIElement> child);
        const std::vector<std::shared_ptr<UIElement>>& getChildren() const { return _children; }

        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed);
        virtual bool containsPoint(const math::Vector2f& point) const;

        void setOnClick(std::function<void()> callback) { _onClick = callback; }
        void setOnHover(std::function<void()> callback) { _onHover = callback; }

        virtual void render();

        virtual void update(float deltaTime);

    protected:
        std::weak_ptr<ecs::ResourceManager> _resourceManager;
        math::Vector2f _position;
        math::Vector2f _size;
        bool _visible = true;
        UIState _state = UIState::Normal;
        std::weak_ptr<UIElement> _parent;
        std::vector<std::shared_ptr<UIElement>> _children;

        std::function<void()> _onClick;
        std::function<void()> _onHover;

        std::pair<int, int> getWindowSize() const;
};

} // namespace ui

#endif /* !UIELEMENT_HPP_ */