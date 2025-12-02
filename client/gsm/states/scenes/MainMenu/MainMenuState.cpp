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
#include "../../../../../common/constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../Dev/DevState.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(resourceManager);

    // Create a test button
    _testButton = std::make_shared<ui::Button>(resourceManager);
    _testButton->setText("Test Button");
    _testButton->setPosition(math::Vector2f(0.4f, 0.4f));
    _testButton->setSize(math::Vector2f(0.2f, 0.1f));
    _testButton->setOnRelease([this]() {
        _gsm->requestStateChange(std::make_shared<DevState>(_gsm, _resourceManager));
    });
}

void MainMenuState::enter() {
    // TODO(anyone): Initialize main menu state
}

void MainMenuState::update(float deltaTime) {
    (void)deltaTime;

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    math::Vector2f mousePos = _mouseHandler->getMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    if (_testButton) {
        _testButton->handleInput(mousePos, mousePressed);
        _testButton->update(deltaTime);
    }

    renderUI();
}

void MainMenuState::renderUI() {
    if (_testButton) {
        _testButton->render();
    }
}

void MainMenuState::exit() {
    _testButton.reset();
    _mouseHandler.reset();
}

}  // namespace gsm
