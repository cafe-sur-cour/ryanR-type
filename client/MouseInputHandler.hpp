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
#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../common/types/Vector2f.hpp"
#include "../../common/constants.hpp"

struct MouseClickInfo {
    math::Vector2f position;
    constants::MouseButton button;
};

class MouseInputHandler {
    public:
        MouseInputHandler(std::shared_ptr<ecs::ResourceManager> resourceManager);
        ~MouseInputHandler() = default;

        std::optional<MouseClickInfo> pollMouseClick();
        math::Vector2f getMousePosition() const;
        math::Vector2f getNormalizedMousePosition() const;
        bool isMouseButtonPressed(int button) const;

    private:
        std::weak_ptr<ecs::ResourceManager> _resourceManager;
};

#endif /* !MOUSEINPUTHANDLER_HPP_ */
