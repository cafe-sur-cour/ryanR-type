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
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return std::nullopt;
    }

    while (true) {
        auto eventResult = resourceManager->get<gfx::IEvent>()->pollEvents();

        if (eventResult == gfx::EventType::MOUSELEFTCLICK ||
            eventResult == gfx::EventType::MOUSERIGHTCLICK ||
            eventResult == gfx::EventType::MOUSEMIDDLECLICK ||
            eventResult == gfx::EventType::MOUSECLICK) {
            auto mousePos = resourceManager->get<gfx::IEvent>()->getMousePos();
            math::Vector2f position(static_cast<float>(mousePos.first),
                                   static_cast<float>(mousePos.second));

            constants::MouseButton button = constants::MouseButton::LEFT;
            if (eventResult == gfx::EventType::MOUSELEFTCLICK)
                button = constants::MouseButton::LEFT;
            else if (eventResult == gfx::EventType::MOUSERIGHTCLICK)
                button = constants::MouseButton::RIGHT;
            else if (eventResult == gfx::EventType::MOUSEMIDDLECLICK)
                button = constants::MouseButton::MIDDLE;

            return MouseClickInfo{position, button};
        } else if (eventResult == gfx::EventType::NOTHING) {
            break;
        }
    }

    return std::nullopt;
}

math::Vector2f MouseInputHandler::getMousePosition() const {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return math::Vector2f(0.0f, 0.0f);
    }
    auto mousePos = resourceManager->get<gfx::IEvent>()->getMousePos();
    return math::Vector2f(static_cast<float>(mousePos.first),
                         static_cast<float>(mousePos.second));
}

math::Vector2f MouseInputHandler::getNormalizedMousePosition() const {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return math::Vector2f(0.0f, 0.0f);
    }

    auto mousePos = getMousePosition();
    auto windowSize = resourceManager->get<gfx::IWindow>()->getWindowSize();

    if (windowSize.first == 0 || windowSize.second == 0) {
        return math::Vector2f(0.0f, 0.0f);
    }

    float normalizedX = mousePos.getX() / static_cast<float>(windowSize.first);
    float normalizedY = mousePos.getY() / static_cast<float>(windowSize.second);

    return math::Vector2f(normalizedX, normalizedY);
}

bool MouseInputHandler::isMouseButtonPressed(int button) const {
    auto resourceManager = _resourceManager.lock();
    if (!resourceManager) {
        return false;
    }
    return resourceManager->get<gfx::IEvent>()->isMouseButtonPressed(button);
}
