/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AFocusableElement - Abstract base class for focusable UI elements
*/

#ifndef AFOCUSABLEELEMENT_HPP_
#define AFOCUSABLEELEMENT_HPP_

#include "../elements/base/UIElement.hpp"
#include "IFocusable.hpp"
#include <functional>

namespace ui {

class AFocusableElement : public UIElement, public IFocusable {
    public:
        explicit AFocusableElement(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~AFocusableElement() = default;

        virtual void setFocused(bool focused) override;
        virtual bool isFocused() const override;
        virtual bool canBeFocused() const override;
        virtual void onFocusGained() override;
        virtual void onFocusLost() override;
        virtual void onActivated() override;

        void setOnFocusGained(std::function<void()> callback);
        void setOnFocusLost(std::function<void()> callback);
        void setOnActivated(std::function<void()> callback);

        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed) override;

    protected:
        bool _focused = false;
        std::function<void()> _onFocusGained;
        std::function<void()> _onFocusLost;
        std::function<void()> _onActivated;

        virtual void onFocusStateChanged(bool focused);
};

} // namespace ui

#endif /* !AFOCUSABLEELEMENT_HPP_ */