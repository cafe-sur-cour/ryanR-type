/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#include "MainMenuState.hpp"
#include <memory>
#include <iostream>
#include <optional>
#include "../../../../../libs/Multimedia/IWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../MouseInputHandler.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ecs::ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(resourceManager);
}

void MainMenuState::enter() {
    // TODO(anyone): Initialize main menu state
}

void MainMenuState::update(float deltaTime) {
    (void)deltaTime;

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();

    if (eventResult == gfx::EventType::MOUSELEFTCLICK ||
        eventResult == gfx::EventType::MOUSERIGHTCLICK ||
        eventResult == gfx::EventType::MOUSEMIDDLECLICK ||
        eventResult == gfx::EventType::MOUSECLICK) {
        math::Vector2f normalizedPos = _mouseHandler->getNormalizedMousePosition();

        const char* buttonNames[] = {"Left", "Right", "Middle", "Unknown"};
        int buttonIndex = 0;
        if (eventResult == gfx::EventType::MOUSELEFTCLICK) buttonIndex = 0;
        else if (eventResult == gfx::EventType::MOUSERIGHTCLICK) buttonIndex = 1;
        else if (eventResult == gfx::EventType::MOUSEMIDDLECLICK) buttonIndex = 2;
        else buttonIndex = 3;

        std::cout << buttonNames[buttonIndex] << " click at normalized position: ("
                  << normalizedPos.getX() << ", " << normalizedPos.getY() << ")" << std::endl;

        auto mousePos = _resourceManager->get<gfx::IEvent>()->getMousePos();
        std::cout << "Absolute position: ("
            << mousePos.first << ", " << mousePos.second << ")" << std::endl;
    } else if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    // TODO(anyone): Update main menu state
}

void MainMenuState::exit() {
    // TODO(anyone): Cleanup main menu state
}

}  // namespace gsm
