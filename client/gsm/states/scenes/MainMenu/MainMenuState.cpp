/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#include "MainMenuState.hpp"
#include <memory>
#include <string>
#include <vector>
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
    : AGameState(gsm, resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Play Game");
    _playButton->setPosition(math::Vector2f(672.f, 378.f));
    _playButton->setSize(math::Vector2f(576.f, 108.f));
    _playButton->setNormalColor({0, 200, 0});
    _playButton->setHoveredColor({0, 255, 0});
    _playButton->setFocusedColor({255, 255, 0});

    _playButton->setOnRelease([this]() {
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
    _playButton->setOnActivated([this]() {
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
    _uiManager->addElement(_playButton);

    _quitButton = std::make_shared<ui::Button>(resourceManager);
    _quitButton->setText("Quit");
    _quitButton->setPosition(math::Vector2f(672.f, 540.f));
    _quitButton->setSize(math::Vector2f(576.f, 108.f));
    _quitButton->setNormalColor({200, 0, 0});
    _quitButton->setHoveredColor({255, 0, 0});
    _quitButton->setFocusedColor({255, 100, 0});
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
    _highContrastButton->setNormalColor({200, 0, 0});
    _highContrastButton->setHoveredColor({150, 150, 200});
    _highContrastButton->setFocusedColor({100, 200, 255});
    _highContrastButton->setOnRelease([this]() {
        toggleHighContrastFilter();
    });
    _highContrastButton->setOnActivated([this]() {
        toggleHighContrastFilter();
    });
    _uiManager->addElement(_highContrastButton);

    _colorBlindnessButton = std::make_shared<ui::Button>(resourceManager);
    _colorBlindnessButton->setText("CB: None");
    _colorBlindnessButton->setPosition(math::Vector2f(1646.f, 22.f));
    _colorBlindnessButton->setSize(math::Vector2f(115.f, 65.f));
    _colorBlindnessButton->setNormalColor({100, 100, 150});
    _colorBlindnessButton->setHoveredColor({150, 150, 200});
    _colorBlindnessButton->setFocusedColor({100, 200, 255});
    _colorBlindnessButton->setOnRelease([this]() {
        cycleColorBlindnessFilter();
    });
    _colorBlindnessButton->setOnActivated([this]() {
        cycleColorBlindnessFilter();
    });
    _uiManager->addElement(_colorBlindnessButton);

    _brightnessButton = std::make_shared<ui::Button>(resourceManager);
    _brightnessButton->setText("BR: 100%");
    _brightnessButton->setPosition(math::Vector2f(1526.f, 22.f));
    _brightnessButton->setSize(math::Vector2f(115.f, 65.f));
    _brightnessButton->setNormalColor({100, 100, 150});
    _brightnessButton->setHoveredColor({150, 150, 200});
    _brightnessButton->setFocusedColor({100, 200, 255});
    _brightnessButton->setOnRelease([this]() {
        cycleBrightnessFilter();
    });
    _brightnessButton->setOnActivated([this]() {
        cycleBrightnessFilter();
    });
    _uiManager->addElement(_brightnessButton);
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

    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (sfmlWindow) {
        std::vector<gfx::color_t> spectrumColors = {
            {255, 0, 0, 255},     // Red
            {255, 165, 0, 255},   // Orange
            {255, 255, 0, 255},   // Yellow
            {0, 255, 0, 255},     // Green
            {0, 255, 255, 255},   // Cyan
            {0, 0, 255, 255},     // Blue
            {128, 0, 128, 255}    // Purple
        };

        size_t barWidth = static_cast<size_t>(constants::WINDOW_WIDTH) / spectrumColors.size();
        size_t barHeight = 50;
        size_t yPos = static_cast<size_t>(constants::WINDOW_HEIGHT) - barHeight;

        for (size_t i = 0; i < spectrumColors.size(); ++i) {
            size_t xPos = i * barWidth;
            sfmlWindow->drawFilledRectangle(
                spectrumColors[i], {xPos, yPos}, {barWidth, barHeight});
        }
    }
}

void MainMenuState::cycleColorBlindnessFilter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    if (_colorBlindnessState == 1) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_PROTANOPIA_SHADER_PATH);
    } else if (_colorBlindnessState == 2) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    } else if (_colorBlindnessState == 3) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_TRITANOPIA_SHADER_PATH);
    }

    _colorBlindnessState = (_colorBlindnessState + 1) % 4;

    if (_colorBlindnessState == 1) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_PROTANOPIA_SHADER_PATH);
        _colorBlindnessButton->setText("CB: Pro");
    } else if (_colorBlindnessState == 2) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_DEUTERANOPIA_SHADER_PATH);
        _colorBlindnessButton->setText("CB: Deu");
    } else if (_colorBlindnessState == 3) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_TRITANOPIA_SHADER_PATH);
        _colorBlindnessButton->setText("CB: Tri");
    } else {
        _colorBlindnessButton->setText("CB: None");
    }
}

void MainMenuState::toggleHighContrastFilter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    bool isActive = sfmlWindow->getShaderManager().isFilterActive(
        constants::FILTER_HIGH_CONTRAST_SHADER_PATH);

    if (isActive) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setNormalColor({200, 0, 0});
    } else {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setNormalColor({0, 200, 0});
    }
}

void MainMenuState::cycleBrightnessFilter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    _brightnessState = (_brightnessState + 1) % 6;

    float brightnessValue;
    std::string text;
    switch (_brightnessState) {
        case 0:
            brightnessValue = 0.25f;
            text = "BR: 25%";
            break;
        case 1:
            brightnessValue = 0.5f;
            text = "BR: 50%";
            break;
        case 2:
            brightnessValue = 0.75f;
            text = "BR: 75%";
            break;
        case 3:
            brightnessValue = 1.0f;
            text = "BR: 100%";
            break;
        case 4:
            brightnessValue = 1.25f;
            text = "BR: 125%";
            break;
        case 5:
            brightnessValue = 1.5f;
            text = "BR: 150%";
            break;
        default:
            brightnessValue = 1.0f;
            text = "BR: 100%";
            break;
    }

    sfmlWindow->getShaderManager().addFilter(constants::FILTER_BRIGHTNESS_SHADER_PATH);
    sfmlWindow->getShaderManager().setUniform(
        constants::FILTER_BRIGHTNESS_SHADER_PATH, "brightness", brightnessValue);

    _brightnessButton->setText(text);
}

void MainMenuState::exit() {
    _uiManager->clearElements();
    _playButton.reset();
    _quitButton.reset();
    _highContrastButton.reset();
    _colorBlindnessButton.reset();
    _brightnessButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
