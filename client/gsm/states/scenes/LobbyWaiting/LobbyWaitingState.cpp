/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyWaitingState
*/

#include "LobbyWaitingState.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../libs/Multimedia/SFML/SfmlWindow.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../colors.hpp"

namespace gsm {

LobbyWaitingState::LobbyWaitingState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isLobbyMaster
) : AGameState(gsm, resourceManager), _isLobbyMaster(isLobbyMaster) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    auto config = _resourceManager->get<SettingsConfig>();
    if (config) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    ui::LayoutConfig centerConfig;
    centerConfig.direction = ui::LayoutDirection::Vertical;
    centerConfig.alignment = ui::LayoutAlignment::Center;
    centerConfig.spacing = 50.0f;
    centerConfig.padding = math::Vector2f(0.0f, 0.0f);
    centerConfig.anchorX = ui::AnchorX::Center;
    centerConfig.anchorY = ui::AnchorY::Center;
    centerConfig.offset = math::Vector2f(0.0f, 0.0f);

    _centerLayout = std::make_shared<ui::UILayout>(_resourceManager, centerConfig);
    _centerLayout->setSize(math::Vector2f(600.f, 400.f));

    if (_isLobbyMaster) {
        setupLobbyMasterUI();
    } else {
        setupPlayerUI();
    }

    _uiManager->addElement(_centerLayout);
}

void LobbyWaitingState::setupLobbyMasterUI() {
    auto networkLobby = _resourceManager->get<ClientNetwork>();
    std::string lobbyCode = networkLobby ? networkLobby->getLobbyCode() : "Unknown";

    _lobbyCodeText = std::make_shared<ui::Text>(_resourceManager);
    _lobbyCodeText->setText("Lobby Code: " + lobbyCode);
    _lobbyCodeText->setSize(math::Vector2f(600.f, 80.f));
    _lobbyCodeText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _lobbyCodeText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _lobbyCodeText->setOutlineThickness(2.0f);
    _lobbyCodeText->setFontSize(48);
    _centerLayout->addElement(_lobbyCodeText);

    _statusText = std::make_shared<ui::Text>(_resourceManager);
    _statusText->setText("Share this code with other players");
    _statusText->setSize(math::Vector2f(600.f, 50.f));
    _statusText->setTextColor(gfx::color_t{200, 200, 200, 255});
    _statusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _statusText->setOutlineThickness(1.0f);
    _statusText->setFontSize(24);
    _centerLayout->addElement(_statusText);

    _startGameButton = std::make_shared<ui::Button>(_resourceManager);
    _startGameButton->setText("Start Game");
    _startGameButton->setSize(math::Vector2f(300.f, 80.f));
    _startGameButton->setNormalColor(colors::BUTTON_PRIMARY);
    _startGameButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _startGameButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);

    _startGameButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendMasterStartGame();
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Master requested to start the game.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _startGameButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendMasterStartGame();
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Master requested to start the game.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _centerLayout->addElement(_startGameButton);
}

void LobbyWaitingState::setupPlayerUI() {
    _statusText = std::make_shared<ui::Text>(_resourceManager);
    _statusText->setText("Waiting for the game to start");
    _statusText->setSize(math::Vector2f(600.f, 80.f));
    _statusText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _statusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _statusText->setOutlineThickness(2.0f);
    _statusText->setFontSize(36);
    _centerLayout->addElement(_statusText);
}

void LobbyWaitingState::enter() {
    if (_isLobbyMaster) {
        auto network = _resourceManager->get<ClientNetwork>();
        if (network && !network->getLobbyCode().empty()) {
            network->sendLobbyConnection(network->getLobbyCode());
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Auto-connecting to lobby as master with code: " +
                network->getLobbyCode(),
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    }
}

void LobbyWaitingState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (config && _uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _uiManager->handleKeyboardInput(eventResult);

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
    updateUIStatus();
    renderUI();
}

void LobbyWaitingState::renderUI() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        auto windowSize = window->getWindowSize();
        window->drawFilledRectangle(
            gfx::color_t{0, 0, 0, 255},
            {0, 0},
            {static_cast<size_t>(windowSize.first), static_cast<size_t>(windowSize.second)}
        );
    }

    _uiManager->render();
}

void LobbyWaitingState::updateUIStatus() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (!network) {
        return;
    }

    if (_isLobbyMaster && _lobbyCodeText) {
        std::string lobbyCode = network->getLobbyCode();
        if (!lobbyCode.empty()) {
            _lobbyCodeText->setText("Lobby Code: " + lobbyCode);
        }
    }

    // Check if we should transition to another state
    // This could be based on game start signal from server
    // For now, we'll stay in this state until manually transitioned
}

void LobbyWaitingState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _lobbyCodeText.reset();
    _statusText.reset();
    _startGameButton.reset();
    _centerLayout.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
