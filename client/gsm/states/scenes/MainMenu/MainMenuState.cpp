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
#include "../../../../../libs/Multimedia/SfmlWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../Dev/DevState.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm), _resourceManager(resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Play Game");
    _playButton->setPosition(math::Vector2f(672.f, 378.f));
    _playButton->setSize(math::Vector2f(576.f, 108.f));
    _playButton->setNormalColor({100, 150, 100});
    _playButton->setHoveredColor({150, 200, 150});
    _playButton->setFocusedColor({255, 215, 0});

    _playButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            this->_gsm->requestStateChange(std::make_shared<DevState>(this->_gsm,
                this->_resourceManager));
        } else {
            debug::Debug::printDebug(network ? network->isDebugMode() : false,
                "[MainMenu] Cannot start game: Not connected to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
    });
    _playButton->setOnActivated([this]() {
        // auto network = this->_resourceManager->get<ClientNetwork>();
        // if (network && network->isConnected()) {
            this->_gsm->requestStateChange(std::make_shared<DevState>(this->_gsm,
                this->_resourceManager));
        // } else {
        //     debug::Debug::printDebug(network ? network->isDebugMode() : false,
        //         "[MainMenu] Cannot start game: Not connected to server.",
        //         debug::debugType::NETWORK,
        //         debug::debugLevel::WARNING);
        // }
    });
    _uiManager->addElement(_playButton);

    _quitButton = std::make_shared<ui::Button>(resourceManager);
    _quitButton->setText("Quit");
    _quitButton->setPosition(math::Vector2f(672.f, 540.f));
    _quitButton->setSize(math::Vector2f(576.f, 108.f));
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

    _highContrastButton = std::make_shared<ui::Button>(resourceManager);
    _highContrastButton->setText("HC");
    _highContrastButton->setPosition(math::Vector2f(1766.f, 22.f));
    _highContrastButton->setSize(math::Vector2f(115.f, 65.f));
    _highContrastButton->setNormalColor({100, 100, 150});
    _highContrastButton->setHoveredColor({150, 150, 200});
    _highContrastButton->setFocusedColor({100, 200, 255});
    _highContrastButton->setOnRelease([this]() {
        auto window = _resourceManager->get<gfx::IWindow>();
        auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
        if (sfmlWindow) {
            if (sfmlWindow->getShaderManager().isFilterActive(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH)) {
                sfmlWindow->getShaderManager().removeFilter(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
            } else {
                sfmlWindow->getShaderManager().addFilter(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
            }
        }
    });
    _highContrastButton->setOnActivated([this]() {
        auto window = _resourceManager->get<gfx::IWindow>();
        auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
        if (sfmlWindow) {
            if (sfmlWindow->getShaderManager().isFilterActive(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH)) {
                sfmlWindow->getShaderManager().removeFilter(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
            } else {
                sfmlWindow->getShaderManager().addFilter(
                    constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
            }
        }
    });
    _uiManager->addElement(_highContrastButton);
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

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
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
    _highContrastButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
