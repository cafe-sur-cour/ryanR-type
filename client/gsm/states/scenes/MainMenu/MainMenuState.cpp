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
#include "../Settings/SettingsState.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig leftConfig;
    leftConfig.direction = ui::LayoutDirection::Vertical;
    leftConfig.alignment = ui::LayoutAlignment::Center;
    leftConfig.spacing = 0.0f;
    leftConfig.padding = math::Vector2f(0.0f, 0.0f);
    leftConfig.anchorX = ui::AnchorX::Left;
    leftConfig.anchorY = ui::AnchorY::Center;
    leftConfig.offset = math::Vector2f(50.0f, 0.0f);

    _leftLayout = std::make_shared<ui::UILayout>(_resourceManager, leftConfig);
    _leftLayout->setSize(math::Vector2f(300.f, 108.f));

    _connectButton = std::make_shared<ui::Button>(_resourceManager);
    _connectButton->setText("Connect to Server");
    _connectButton->setSize(math::Vector2f(300.f, 108.f));

    _connectButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && !network->isConnected()) {
            network->connect();
            debug::Debug::printDebug(network->isDebugMode(),
                "[MainMenu] Connecting to server...",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _connectButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && !network->isConnected()) {
            network->connect();
            debug::Debug::printDebug(network->isDebugMode(),
                "[MainMenu] Connecting to server...",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _leftLayout->addElement(_connectButton);

    ui::LayoutConfig menuConfig;
    menuConfig.direction = ui::LayoutDirection::Vertical;
    menuConfig.alignment = ui::LayoutAlignment::Center;
    menuConfig.spacing = 54.0f;
    menuConfig.padding = math::Vector2f(0.0f, 0.0f);
    menuConfig.anchorX = ui::AnchorX::Center;
    menuConfig.anchorY = ui::AnchorY::Center;
    menuConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainMenuLayout = std::make_shared<ui::UILayout>(_resourceManager, menuConfig);
    _mainMenuLayout->setSize(math::Vector2f(576.f, 400.f));
    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Ready");
    _playButton->setSize(math::Vector2f(576.f, 108.f));

    _playButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendReady();
            debug::Debug::printDebug(network->isDebugMode(),
                "[MainMenu] Sent ready signal to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        } else {
            debug::Debug::printDebug(network ? network->isDebugMode() : false,
                "[MainMenu] Cannot send ready: Not connected to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
    });
    _playButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendReady();
            debug::Debug::printDebug(network->isDebugMode(),
                "[MainMenu] Sent ready signal to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        } else {
            debug::Debug::printDebug(network ? network->isDebugMode() : false,
                "[MainMenu] Cannot send ready: Not connected to server.",
                debug::debugType::NETWORK,
                debug::debugLevel::WARNING);
        }
    });

    _settingsButton = std::make_shared<ui::Button>(resourceManager);
    _settingsButton->setText("Settings");
    _settingsButton->setSize(math::Vector2f(576.f, 108.f));
    _settingsButton->setNormalColor(colors::BUTTON_SECONDARY);
    _settingsButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _settingsButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _settingsButton->setOnRelease([this]() {
        this->_gsm->requestStatePush(std::make_shared<SettingsState>(this->_gsm,
            this->_resourceManager));
    });
    _settingsButton->setOnActivated([this]() {
        this->_gsm->requestStatePush(std::make_shared<SettingsState>(this->_gsm,
            this->_resourceManager));
    });

    _quitButton = std::make_shared<ui::Button>(resourceManager);
    _quitButton->setText("Quit");
    _quitButton->setSize(math::Vector2f(576.f, 108.f));
    _quitButton->setNormalColor(colors::BUTTON_SECONDARY);
    _quitButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _quitButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _quitButton->setOnRelease([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });
    _quitButton->setOnActivated([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });

    _mainMenuLayout->addElement(_playButton);
    _mainMenuLayout->addElement(_settingsButton);
    _mainMenuLayout->addElement(_quitButton);

    ui::LayoutConfig rightConfig;
    rightConfig.direction = ui::LayoutDirection::Vertical;
    rightConfig.alignment = ui::LayoutAlignment::Center;
    rightConfig.spacing = 50.0f;
    rightConfig.padding = math::Vector2f(0.0f, 0.0f);
    rightConfig.anchorX = ui::AnchorX::Right;
    rightConfig.anchorY = ui::AnchorY::Center;
    rightConfig.offset = math::Vector2f(-50.0f, 0.0f);

    _rightLayout = std::make_shared<ui::UILayout>(_resourceManager, rightConfig);
    _rightLayout->setSize(math::Vector2f(400.f, 108.f));

    _devButton = std::make_shared<ui::Button>(_resourceManager);
    _devButton->setText("Go to dev scene\n(no need to connect to server)");
    _devButton->setSize(math::Vector2f(400.f, 108.f));
    _devButton->setNormalColor(colors::BUTTON_PRIMARY);
    _devButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _devButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _devButton->setOnRelease([this]() {
        this->_gsm->requestStatePush(std::make_shared<DevState>(this->_gsm,
            this->_resourceManager));
    });
    _devButton->setOnActivated([this]() {
        this->_gsm->requestStatePush(std::make_shared<DevState>(this->_gsm,
            this->_resourceManager));
    });
    _rightLayout->addElement(_devButton);

    _uiManager->addElement(_leftLayout);
    _uiManager->addElement(_mainMenuLayout);
    _uiManager->addElement(_rightLayout);
}

void MainMenuState::enter() {
}

void MainMenuState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (_uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _uiManager->handleMouseInput(mousePos, mousePressed);

    bool isHoveringUI = _uiManager->isMouseHoveringAnyElement(mousePos);
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

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
    _settingsButton.reset();
    _quitButton.reset();
    _connectButton.reset();
    _mainMenuLayout.reset();
    _rightLayout.reset();
    _leftLayout.reset();
    _background.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
