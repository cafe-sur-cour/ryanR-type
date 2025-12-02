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
#include "../../../../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../Dev/DevState.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Play Game");
    _playButton->setPosition(math::Vector2f(0.35f, 0.35f));
    _playButton->setSize(math::Vector2f(0.3f, 0.1f));
    _playButton->setNormalColor({100, 150, 100});
    _playButton->setHoveredColor({150, 200, 150});
    _playButton->setFocusedColor({255, 215, 0});
    _playButton->setOnRelease([this]() {
        _gsm->requestStateChange(std::make_shared<DevState>(_gsm, _resourceManager));
    });
    _playButton->setOnActivated([this]() {
        _gsm->requestStateChange(std::make_shared<DevState>(_gsm, _resourceManager));
    });
    _uiManager->addElement(_playButton);

    _quitButton = std::make_shared<ui::Button>(resourceManager);
    _quitButton->setText("Quit");
    _quitButton->setPosition(math::Vector2f(0.35f, 0.5f));
    _quitButton->setSize(math::Vector2f(0.3f, 0.1f));
    _quitButton->setNormalColor({150, 100, 100});
    _quitButton->setHoveredColor({200, 150, 150});
    _quitButton->setFocusedColor({255, 69, 0});
    _quitButton->setOnRelease([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });
    _quitButton->setOnActivated([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });
    _uiManager->addElement(_quitButton);
}

void MainMenuState::enter() {
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

    _uiManager->handleMouseInput(mousePos, mousePressed);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void MainMenuState::renderUI() {
    _uiManager->render();
}

void MainMenuState::exit() {
    _uiManager->clearElements();
    _playButton.reset();
    _quitButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
