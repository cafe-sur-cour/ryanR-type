/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PauseState
*/

#include "PauseState.hpp"
#include <memory>
#include <string>
#include "../../../../colors.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../MainMenu/MainMenuState.hpp"
#include "../Settings/SettingsState.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../input/MouseInputHandler.hpp"

namespace gsm {

PauseState::PauseState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
    _uiManager = std::make_unique<ui::UIManager>();
}

void PauseState::enter() {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);

    auto window = _resourceManager->get<gfx::IWindow>();
    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    ui::LayoutConfig menuConfig;
    menuConfig.direction = ui::LayoutDirection::Vertical;
    menuConfig.alignment = ui::LayoutAlignment::Center;
    menuConfig.spacing = 20.0f;
    menuConfig.padding = math::Vector2f(20.0f, 20.0f);
    menuConfig.anchorX = ui::AnchorX::Center;
    menuConfig.anchorY = ui::AnchorY::Center;
    _menuLayout = std::make_shared<ui::UILayout>(_resourceManager, menuConfig);

    _resumeButton = std::make_shared<ui::Button>(_resourceManager);
    _resumeButton->setText("Resume");
    _resumeButton->setSize(math::Vector2f(250.f, 60.f));
    _resumeButton->setNormalColor(colors::BUTTON_PRIMARY);
    _resumeButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _resumeButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);

    _resumeButton->setOnRelease([this]() {
        if (auto gsm = _gsm.lock()) {
            gsm->requestStatePop();
        }
    });
    _resumeButton->setOnActivated([this]() {
        if (auto gsm = _gsm.lock()) {
            gsm->requestStatePop();
        }
    });
    _menuLayout->addElement(_resumeButton);

    _settingsButton = std::make_shared<ui::Button>(_resourceManager);
    _settingsButton->setText("Settings");
    _settingsButton->setSize(math::Vector2f(250.f, 60.f));
    _settingsButton->setNormalColor(colors::BUTTON_PRIMARY);
    _settingsButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _settingsButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);

    _settingsButton->setOnRelease([this]() {
        if (auto gsm = _gsm.lock()) {
            gsm->requestStatePush(std::make_shared<SettingsState>(gsm, _resourceManager));
        }
    });
    _settingsButton->setOnActivated([this]() {
        if (auto gsm = _gsm.lock()) {
            gsm->requestStatePush(std::make_shared<SettingsState>(gsm, _resourceManager));
        }
    });
    _menuLayout->addElement(_settingsButton);

    _leaveButton = std::make_shared<ui::Button>(_resourceManager);
    _leaveButton->setText("Leave");
    _leaveButton->setSize(math::Vector2f(250.f, 60.f));
    _leaveButton->setNormalColor(colors::BUTTON_DANGER);
    _leaveButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _leaveButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);

    _leaveButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[PauseState] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _leaveButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[PauseState] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _menuLayout->addElement(_leaveButton);

    _uiManager->addElement(_menuLayout);

    _uiManager->setNavigationEnabled(true);
    _uiManager->focusFirstElement();

    if (_resourceManager->has<ClientNetwork>()) {
        NetworkEvent stopEvent;
        stopEvent.eventType = constants::EventType::STOP;
        stopEvent.depth = 0.0;
        _resourceManager->get<ClientNetwork>()->addToEventQueue(stopEvent);
    }
}

void PauseState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (_uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    auto network = this->_resourceManager->get<ClientNetwork>();
    if (network && network->_shouldDisconnect) {
        network->setLobbyCode("");
        network->_isConnectedToLobby = false;
        network->_isLobbyMaster = false;
        network->_ready = false;
        network->clearEntitiesAndMappings();

        if (auto gsm = _gsm.lock()) {
            gsm->requestStateChange(std::make_shared<MainMenuState>(gsm, _resourceManager));
        }

        auto IAudio = this->_resourceManager->get<gfx::IAudio>();
        if (IAudio) {
            IAudio->stopAllSounds();
            IAudio->stopMusic();
        }

        network->_shouldDisconnect = false;

        debug::Debug::printDebug(network->isDebugMode(),
            "[PauseState] Disconnection ACK received; changing to MainMenuState.",
            debug::debugType::NETWORK,
            debug::debugLevel::INFO);
    }
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _uiManager->handleKeyboardInput(eventResult);

    if (eventResult == gfx::EventType::TEXT_INPUT) {
        std::string textInput = _resourceManager->get<gfx::IEvent>()->getLastTextInput();
        if (!textInput.empty()) {
            _uiManager->handleTextInput(textInput);
        }
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::LEFT));
    _uiManager->handleMouseInput(mousePos, mousePressed);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
            if (auto gsm = _gsm.lock()) {
                gsm->requestStatePop();
                return;
            }
        }

        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void PauseState::renderUI() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        auto windowSize = window->getWindowSize();
        window->drawFilledRectangle(
            gfx::color_t{0, 0, 0, 128},
            {0, 0},
            {static_cast<size_t>(windowSize.first), static_cast<size_t>(windowSize.second)}
        );
    }

    _uiManager->render();
}

void PauseState::exit() {
    if (_resourceManager->has<gfx::IWindow>()) {
        auto window = _resourceManager->get<gfx::IWindow>();
        window->setCursor(false);
    }
    _uiManager->clearElements();
    _menuLayout.reset();
    _resumeButton.reset();
    _settingsButton.reset();
    _leaveButton.reset();
}

}  // namespace gsm
