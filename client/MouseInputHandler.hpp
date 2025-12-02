/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MouseInputHandler
*/

#ifndef MOUSEINPUTHANDLER_HPP_
#define MOUSEINPUTHANDLER_HPP_

#include <memory>
#include <optional>
#include "../../common/resourceManager/ResourceManager.hpp"
#include "../../common/types/Vector2f.hpp"
#include "constants.hpp"

struct MouseClickInfo {
    math::Vector2f position;
    constants::MouseButton button;
};

class MouseInputHandler {
    public:
        MouseInputHandler(std::shared_ptr<ResourceManager> resourceManager);
        ~MouseInputHandler() = default;

        std::optional<MouseClickInfo> pollMouseClick();
        math::Vector2f getMousePosition() const;
        math::Vector2f getNormalizedMousePosition() const;
        bool isMouseButtonPressed(int button) const;

    private:
        std::weak_ptr<ResourceManager> _resourceManager;
};

#endif /* !MOUSEINPUTHANDLER_HPP_ */
