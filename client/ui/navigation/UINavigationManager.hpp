/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UINavigationManager - Manages UI navigation for keyboard and gamepad
*/

#ifndef UINAVIGATIONMANAGER_HPP_
#define UINAVIGATIONMANAGER_HPP_

#include <vector>
#include <memory>
#include <functional>
#include "../core/IFocusable.hpp"
#include "../../../common/ECS/resourceManager/InputAction.hpp"
#include "../../../common/types/Vector2f.hpp"

namespace ui {

enum class NavigationDirection {
    Up,
    Down,
    Left,
    Right
};

class UINavigationManager {
    public:
        UINavigationManager();
        ~UINavigationManager() = default;

        void addFocusableElement(std::shared_ptr<IFocusable> element);
        void removeFocusableElement(std::shared_ptr<IFocusable> element);
        void clearFocusableElements();

        bool handleNavigationInput(ecs::InputAction action);

        bool setFocus(std::shared_ptr<IFocusable> element);
        std::shared_ptr<IFocusable> getFocusedElement() const;
        void clearFocus();

        bool focusFirstElement();
        bool focusNextElement();
        bool focusPreviousElement();

        void setNavigationEnabled(bool enabled);
        bool isNavigationEnabled() const;

        void setOnFocusChanged(std::function<void(std::shared_ptr<IFocusable>)> callback);

        void onMouseMovement();

        void enableFocus();

        bool isFocusDisabled() const;

    private:
        std::vector<std::weak_ptr<IFocusable>> _focusableElements;
        std::weak_ptr<IFocusable> _currentFocused;
        bool _navigationEnabled;
        bool _focusDisabled;
        std::function<void(std::shared_ptr<IFocusable>)> _onFocusChanged;

        void cleanupExpiredElements();
        int getCurrentFocusedIndex() const;
        bool navigateInDirection(NavigationDirection direction);
};

}  // namespace ui

#endif /* !UINAVIGATIONMANAGER_HPP_ */
