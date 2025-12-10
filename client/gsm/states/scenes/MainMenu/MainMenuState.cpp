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

    ui::LayoutConfig menuConfig;
    menuConfig.direction = ui::LayoutDirection::Vertical;
    menuConfig.alignment = ui::LayoutAlignment::Center;
    menuConfig.spacing = 54.0f;
    menuConfig.padding = math::Vector2f(0.0f, 0.0f);
    menuConfig.anchorX = ui::AnchorX::Center;
    menuConfig.anchorY = ui::AnchorY::Center;
    menuConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainMenuLayout = std::make_shared<ui::UILayout>(resourceManager, menuConfig);
    _mainMenuLayout->setSize(math::Vector2f(576.f, 300.f));
    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Play Game");
    _playButton->setSize(math::Vector2f(576.f, 108.f));
    _playButton->setNormalColor({0, 200, 0});
    _playButton->setHoveredColor({0, 255, 0});
    _playButton->setFocusedColor({255, 255, 0});

    _playButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isReady()) {
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
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isReady()) {
            this->_gsm->requestStateChange(std::make_shared<DevState>(this->_gsm,
                this->_resourceManager));
        } else {
            debug::Debug::printDebug(network ? network->isDebugMode() : false,
                "[MainMenu] Cannot start game: Not connected to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
    });

    _quitButton = std::make_shared<ui::Button>(resourceManager);
    _quitButton->setText("Quit");
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

    _mainMenuLayout->addElement(_playButton);
    _mainMenuLayout->addElement(_quitButton);
    _uiManager->addElement(_mainMenuLayout);

    ui::LayoutConfig scaleConfig;
    scaleConfig.direction = ui::LayoutDirection::Horizontal;
    scaleConfig.alignment = ui::LayoutAlignment::Start;
    scaleConfig.spacing = 5.0f;
    scaleConfig.padding = math::Vector2f(0.0f, 0.0f);
    scaleConfig.anchorX = ui::AnchorX::Left;
    scaleConfig.anchorY = ui::AnchorY::Top;
    scaleConfig.offset = math::Vector2f(15.0f, 22.0f);

    _scaleLayout = std::make_shared<ui::UILayout>(resourceManager, scaleConfig);
    _scaleLayout->setSize(math::Vector2f(115.f, 65.f));

    _scaleButton = std::make_shared<ui::Button>(resourceManager);
    _scaleButton->setText("UI: Normal");
    _scaleButton->setSize(math::Vector2f(115.f, 65.f));
    _scaleButton->setNormalColor({150, 100, 200});
    _scaleButton->setHoveredColor({200, 150, 255});
    _scaleButton->setFocusedColor({255, 200, 255});
    _scaleButton->setOnRelease([this]() {
        cycleUIScale();
    });
    _scaleButton->setOnActivated([this]() {
        cycleUIScale();
    });

    _scaleLayout->addElement(_scaleButton);
    _uiManager->addElement(_scaleLayout);

    ui::LayoutConfig toolbarConfig;
    toolbarConfig.direction = ui::LayoutDirection::Horizontal;
    toolbarConfig.alignment = ui::LayoutAlignment::Start;
    toolbarConfig.spacing = 5.0f;
    toolbarConfig.padding = math::Vector2f(0.0f, 0.0f);
    toolbarConfig.anchorX = ui::AnchorX::Right;
    toolbarConfig.anchorY = ui::AnchorY::Top;
    toolbarConfig.offset = math::Vector2f(-15.0f, 22.0f);

    _toolbarLayout = std::make_shared<ui::UILayout>(resourceManager, toolbarConfig);
    _toolbarLayout->setSize(math::Vector2f(400.f, 65.f));

    _brightnessButton = std::make_shared<ui::Button>(resourceManager);
    _brightnessButton->setText("BR: 100%");
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

    _colorBlindnessButton = std::make_shared<ui::Button>(resourceManager);
    _colorBlindnessButton->setText("CB: None");
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

    _highContrastButton = std::make_shared<ui::Button>(resourceManager);
    _highContrastButton->setText("HC");
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

    _toolbarLayout->addElement(_brightnessButton);
    _toolbarLayout->addElement(_colorBlindnessButton);
    _toolbarLayout->addElement(_highContrastButton);
    _uiManager->addElement(_toolbarLayout);
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

void MainMenuState::cycleUIScale() {
    _uiManager->cycleGlobalScale();

    std::string text;
    switch (_uiManager->getGlobalScale()) {
        case ui::UIScale::Small:
            text = "UI: Small";
            break;
        case ui::UIScale::Normal:
            text = "UI: Normal";
            break;
        case ui::UIScale::Large:
            text = "UI: Large";
            break;
        default:
            text = "UI: Normal";
            break;
    }

    _scaleButton->setText(text);
}

void MainMenuState::exit() {
    _uiManager->clearElements();
    _playButton.reset();
    _quitButton.reset();
    _highContrastButton.reset();
    _colorBlindnessButton.reset();
    _brightnessButton.reset();
    _scaleButton.reset();
    _scaleLayout.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
