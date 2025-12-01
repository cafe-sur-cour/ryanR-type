/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MouseInputHandler
*/

#include "MouseInputHandler.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IEvent.hpp"

MouseInputHandler::MouseInputHandler(std::shared_ptr<ecs::ResourceManager> resourceManager)
    : _resourceManager(resourceManager) {
}

std::optional<MouseClickInfo> MouseInputHandler::pollMouseClick() {
    while (true) {
        auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();

        if (eventResult == gfx::EventType::MOUSELEFTCLICK ||
            eventResult == gfx::EventType::MOUSERIGHTCLICK ||
            eventResult == gfx::EventType::MOUSEMIDDLECLICK ||
            eventResult == gfx::EventType::MOUSECLICK) {
            auto mousePos = _resourceManager->get<gfx::IEvent>()->getMousePos();
            math::Vector2f position(static_cast<float>(mousePos.first),
                                   static_cast<float>(mousePos.second));

            int button = 0;
            if (eventResult == gfx::EventType::MOUSELEFTCLICK) button = 0;
            else if (eventResult == gfx::EventType::MOUSERIGHTCLICK) button = 1;
            else if (eventResult == gfx::EventType::MOUSEMIDDLECLICK) button = 2;

            return MouseClickInfo{position, button};
        } else if (eventResult == gfx::EventType::NOTHING) {
            break;
        }
    }

    return std::nullopt;
}

math::Vector2f MouseInputHandler::getMousePosition() const {
    auto mousePos = _resourceManager->get<gfx::IEvent>()->getMousePos();
    return math::Vector2f(static_cast<float>(mousePos.first),
                         static_cast<float>(mousePos.second));
}

math::Vector2f MouseInputHandler::getNormalizedMousePosition() const {
    auto mousePos = getMousePosition();
    auto windowSize = _resourceManager->get<gfx::IWindow>()->getWindowSize();

    if (windowSize.first == 0 || windowSize.second == 0) {
        return math::Vector2f(0.0f, 0.0f);
    }

    float normalizedX = mousePos.getX() / static_cast<float>(windowSize.first);
    float normalizedY = mousePos.getY() / static_cast<float>(windowSize.second);

    return math::Vector2f(normalizedX, normalizedY);
}
