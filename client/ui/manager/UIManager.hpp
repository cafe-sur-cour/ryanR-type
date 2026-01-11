/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UIManager
*/

#ifndef UIMANAGER_HPP_
#define UIMANAGER_HPP_

#include <memory>
#include <vector>
#include <set>
#include "../elements/base/UIElement.hpp"
#include "../navigation/UINavigationManager.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../client/constants.hpp"

namespace ui {

class UIManager {
    public:
        UIManager();
        ~UIManager() = default;

        void addElement(std::shared_ptr<UIElement> element);
        void removeElement(std::shared_ptr<UIElement> element);
        void clearElements();

        void update(float deltaTime);

        void render();

        void handleMouseInput(const math::Vector2f& mousePos, bool mousePressed);
        bool handleNavigationInput(ecs::InputAction action);
        bool handleNavigationInputs(std::shared_ptr<ecs::IInputProvider> inputProvider, float deltaTime);
        void handleKeyboardInput(gfx::EventType event);
        void handleTextInput(const std::string& text);

        std::shared_ptr<UINavigationManager> getNavigationManager();

        void setNavigationEnabled(bool enabled);
        bool isNavigationEnabled() const;

        bool focusFirstElement();
        void clearFocus();

        std::shared_ptr<IFocusable> getFocusedElement() const;

        void setGlobalScale(UIScale scale);
        void cycleGlobalScale();
        UIScale getGlobalScale() const;

        void setOnBack(std::function<void()> callback);

        void setCursorCallback(std::function<void(bool)> callback);

        bool isMouseHoveringAnyElement(const math::Vector2f& mousePos) const;

    private:
        std::vector<std::shared_ptr<UIElement>> _elements;
        std::shared_ptr<UINavigationManager> _navigationManager;
        math::Vector2f _lastMousePos;
        bool _mouseMovementDetected;

        float _navigationCooldown = 0.0f;
        UIScale _globalScale = UIScale::Normal;
        std::function<void()> _onBack;
        std::function<void(bool)> _cursorCallback;
        bool _textInputNavigationBlocked = false;
        std::set<gfx::EventType> _consumedTextKeys;
        std::set<ecs::InputAction> _blockedActions;

        bool hasMouseMoved(const math::Vector2f& mousePos);

        void refreshNavigationElements();
};

} // namespace ui

#endif /* !UIMANAGER_HPP_ */
