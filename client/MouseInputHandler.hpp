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

struct MouseClickInfo {
    math::Vector2f position;
    int button; // 0=left, 1=right, 2=middle
};

class MouseInputHandler {
    public:
        MouseInputHandler(std::shared_ptr<ecs::ResourceManager> resourceManager);
        ~MouseInputHandler() = default;

        std::optional<MouseClickInfo> pollMouseClick();
        math::Vector2f getMousePosition() const;
        math::Vector2f getNormalizedMousePosition() const;

    private:
        std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

#endif /* !MOUSEINPUTHANDLER_HPP_ */