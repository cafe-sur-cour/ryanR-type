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
#include "../../../../../common/GameRules.hpp"
#include "../MainMenu/MainMenuState.hpp"
#include "../Chat/ChatState.hpp"

namespace gsm {

LobbyWaitingState::LobbyWaitingState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isLobbyMaster
) : AGameState(gsm, resourceManager), _isLobbyMaster(isLobbyMaster) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

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

    ui::LayoutConfig topLeftConfig;
    topLeftConfig.direction = ui::LayoutDirection::Vertical;
    topLeftConfig.alignment = ui::LayoutAlignment::Start;
    topLeftConfig.spacing = 10.0f;
    topLeftConfig.padding = math::Vector2f(0.0f, 0.0f);
    topLeftConfig.anchorX = ui::AnchorX::Left;
    topLeftConfig.anchorY = ui::AnchorY::Top;
    topLeftConfig.offset = math::Vector2f(20.0f, 20.0f);

    _topLeftLayout = std::make_shared<ui::UILayout>(_resourceManager, topLeftConfig);
    _topLeftLayout->setSize(math::Vector2f(200.f, 100.f));

    auto gameRules = _resourceManager->get<GameRules>();

    _gamemodeLabel = std::make_shared<ui::Text>(_resourceManager);
    _gamemodeLabel->setText("Gamemode");
    _gamemodeLabel->setSize(math::Vector2f(200.f, 30.f));
    _gamemodeLabel->setTextColor(gfx::color_t{255, 255, 255, 255});
    _gamemodeLabel->setFontSize(24);
    _topLeftLayout->addElement(_gamemodeLabel);

    _gamemodeButton = std::make_shared<ui::Button>(_resourceManager);
    std::string gamemodeText = "Classic";
    if (gameRules) {
        GameRulesNS::Gamemode g = gameRules->getGamemode();
        if (g == GameRulesNS::Gamemode::INFINITE_MODE) gamemodeText = "Infinite";
    }
    _gamemodeButton->setText(gamemodeText);
    _gamemodeButton->setSize(math::Vector2f(150.f, 50.f));
    _gamemodeButton->setNormalColor(colors::BUTTON_PRIMARY);
    _gamemodeButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _gamemodeButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _gamemodeButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendRequestGameRulesUpdate(0, 0);
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Requested gamemode change",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _topLeftLayout->addElement(_gamemodeButton);

    _difficultyLabel = std::make_shared<ui::Text>(_resourceManager);
    _difficultyLabel->setText("Difficulty");
    _difficultyLabel->setSize(math::Vector2f(200.f, 30.f));
    _difficultyLabel->setTextColor(gfx::color_t{255, 255, 255, 255});
    _difficultyLabel->setFontSize(24);
    _topLeftLayout->addElement(_difficultyLabel);

    _difficultyButton = std::make_shared<ui::Button>(_resourceManager);
    std::string diffText = "Normal";
    if (gameRules) {
        GameRulesNS::Difficulty d = gameRules->getDifficulty();
        if (d == GameRulesNS::Difficulty::EASY) diffText = "Easy";
        else if (d == GameRulesNS::Difficulty::HARD) diffText = "Hard";
    }
    _difficultyButton->setText(diffText);
    _difficultyButton->setSize(math::Vector2f(150.f, 50.f));
    _difficultyButton->setNormalColor(colors::BUTTON_PRIMARY);
    _difficultyButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _difficultyButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _difficultyButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendRequestGameRulesUpdate(1, 0);
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Requested game rules change (difficulty)",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _topLeftLayout->addElement(_difficultyButton);

    _crossfireLabel = std::make_shared<ui::Text>(_resourceManager);
    _crossfireLabel->setText("Crossfire");
    _crossfireLabel->setSize(math::Vector2f(200.f, 30.f));
    _crossfireLabel->setTextColor(gfx::color_t{255, 255, 255, 255});
    _crossfireLabel->setFontSize(24);
    _topLeftLayout->addElement(_crossfireLabel);

    _crossfireButton = std::make_shared<ui::Button>(_resourceManager);
    std::string crossfireText = "No";
    if (gameRules) {
        crossfireText = gameRules->getCrossfire() ? "Yes" : "No";
    }
    _crossfireButton->setText(crossfireText);
    _crossfireButton->setSize(math::Vector2f(150.f, 50.f));
    _crossfireButton->setNormalColor(colors::BUTTON_PRIMARY);
    _crossfireButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _crossfireButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _crossfireButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendRequestGameRulesUpdate(2, 0);
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Requested crossfire toggle",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _topLeftLayout->addElement(_crossfireButton);

    _uiManager->addElement(_topLeftLayout);

    ui::LayoutConfig topConfig;
    topConfig.direction = ui::LayoutDirection::Horizontal;
    topConfig.alignment = ui::LayoutAlignment::End;
    topConfig.spacing = 10.0f;
    topConfig.padding = math::Vector2f(0.0f, 0.0f);
    topConfig.anchorX = ui::AnchorX::Right;
    topConfig.anchorY = ui::AnchorY::Top;
    topConfig.offset = math::Vector2f(-20.0f, 20.0f);
    _topRightLayout = std::make_shared<ui::UILayout>(_resourceManager, topConfig);
    _topRightLayout->setSize(math::Vector2f(80.f, 80.f));

    _chatButton = std::make_shared<ui::Button>(_resourceManager);
    _chatButton->setSize(math::Vector2f(80.f, 80.f));
    _chatButton->setIconPath(constants::CHAT_PATH);
    _chatButton->setIconSize(math::Vector2f(500.f, 500.f));
    _chatButton->setNormalColor(colors::BUTTON_SECONDARY);
    _chatButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _chatButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _chatButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<ChatState>(stateMachine,
                this->_resourceManager));
        }
    });
    _chatButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<ChatState>(stateMachine,
                this->_resourceManager));
        }
    });

    _chatButton = std::make_shared<ui::Button>(_resourceManager);
    _chatButton->setSize(math::Vector2f(80.f, 80.f));
    _chatButton->setIconPath(constants::CHAT_PATH);
    _chatButton->setIconSize(math::Vector2f(500.f, 500.f));
    _chatButton->setNormalColor(colors::BUTTON_SECONDARY);
    _chatButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _chatButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _chatButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<ChatState>(stateMachine,
                this->_resourceManager));
        }
    });
    _chatButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<ChatState>(stateMachine,
                this->_resourceManager));
        }
    });

    // create leave button and add both leave + chat to the same top-right layout
    _leaveButton = std::make_shared<ui::Button>(_resourceManager);
    _leaveButton->setText("Leave");
    _leaveButton->setSize(math::Vector2f(150.f, 50.f));
    _leaveButton->setNormalColor(colors::BUTTON_DANGER);
    _leaveButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _leaveButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);

    _leaveButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });
    _leaveButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _topRightLayout->addElement(_leaveButton);
    _topRightLayout->addElement(_chatButton);
    _uiManager->addElement(_topRightLayout);

    _loadingAnimation = std::make_shared<ui::SpritePreview>(_resourceManager);
    if (_loadingAnimation->loadPrefab(constants::LOADING_PREFAVB)) {
        _loadingAnimation->setSize(math::Vector2f(185.0f, 320.0f));
    } else {
        std::cerr << "Failed to load loading animation prefab" << std::endl;
        _loadingAnimation.reset();
    }

    ui::LayoutConfig bottomRightConfig;
    bottomRightConfig.direction = ui::LayoutDirection::Vertical;
    bottomRightConfig.alignment = ui::LayoutAlignment::End;
    bottomRightConfig.spacing = 0.0f;
    bottomRightConfig.padding = math::Vector2f(0.0f, 0.0f);
    bottomRightConfig.anchorX = ui::AnchorX::Right;
    bottomRightConfig.anchorY = ui::AnchorY::Bottom;
    bottomRightConfig.offset = math::Vector2f(-20.0f, -20.0f);

    _loadingLayout = std::make_shared<ui::UILayout>(_resourceManager, bottomRightConfig);
    _loadingLayout->setSize(math::Vector2f(185.0f, 320.0f));
    if (_loadingAnimation) {
        _loadingLayout->addElement(_loadingAnimation);
        _uiManager->addElement(_loadingLayout);
    }
}

void LobbyWaitingState::setupLobbyMasterUI() {
    ui::LayoutConfig bottomLeftConfig;
    bottomLeftConfig.direction = ui::LayoutDirection::Vertical;
    bottomLeftConfig.alignment = ui::LayoutAlignment::Start;
    bottomLeftConfig.spacing = 10.0f;
    bottomLeftConfig.padding = math::Vector2f(0.0f, 0.0f);
    bottomLeftConfig.anchorX = ui::AnchorX::Left;
    bottomLeftConfig.anchorY = ui::AnchorY::Bottom;
    bottomLeftConfig.offset = math::Vector2f(20.0f, -20.0f);

    _bottomLeftLayout = std::make_shared<ui::UILayout>(_resourceManager, bottomLeftConfig);
    _bottomLeftLayout->setSize(math::Vector2f(200.f, 100.f));

    _uiManager->addElement(_bottomLeftLayout);

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

    _copyCodeButton = std::make_shared<ui::Button>(_resourceManager);
    _copyCodeButton->setText("Copy Code");
    _copyCodeButton->setSize(math::Vector2f(150.f, 50.f));
    _copyCodeButton->setNormalColor(colors::BUTTON_SECONDARY);
    _copyCodeButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _copyCodeButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _copyCodeButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        auto window = this->_resourceManager->get<gfx::IWindow>();
        if (network && window && !network->getLobbyCode().empty()) {
            window->setClipboardText(network->getLobbyCode());
            _copyCodeButton->setText("Copied");
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Copied lobby code to clipboard: " +
                    network->getLobbyCode(),
                debug::debugType::CORE,
                debug::debugLevel::INFO);
        }
    });
    _copyCodeButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        auto window = this->_resourceManager->get<gfx::IWindow>();
        if (network && window && !network->getLobbyCode().empty()) {
            window->setClipboardText(network->getLobbyCode());
            _copyCodeButton->setText("Copied");
            debug::Debug::printDebug(network->isDebugMode(),
                "[LobbyWaiting] Copied lobby code to clipboard: " +
                    network->getLobbyCode(),
                debug::debugType::CORE,
                debug::debugLevel::INFO);
        }
    });
    _bottomLeftLayout->addElement(_copyCodeButton);

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
    auto network = this->_resourceManager->get<ClientNetwork>();
    if (network && network->_shouldDisconnect) {
        network->setLobbyCode("");
        network->_isConnectedToLobby = false;
        network->_isLobbyMaster = false;
        network->_ready = false;
        network->clearEntitiesAndMappings();

        auto gsmPtr = _gsm.lock();
        if (gsmPtr) {
            auto mainMenuState =
            std::make_shared<gsm::MainMenuState>(gsmPtr, _resourceManager);
            gsmPtr->requestStateChange(mainMenuState);
        }
        network->_shouldDisconnect = false;
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

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    if (_loadingAnimation) {
        _loadingAnimation->update(deltaTime);
    }
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

    if (_gamemodeButton && _resourceManager->has<GameRules>()) {
        auto gameRules = _resourceManager->get<GameRules>();
        GameRulesNS::Gamemode g = gameRules->getGamemode();
        std::string gamemodeText =
            g == GameRulesNS::Gamemode::CLASSIC ? "Classic" : "Infinite";
        _gamemodeButton->setText(gamemodeText);
    }

    if (_difficultyButton && _resourceManager->has<GameRules>()) {
        auto gameRules = _resourceManager->get<GameRules>();
        GameRulesNS::Difficulty d = gameRules->getDifficulty();
        std::string diffText = "Normal";
        if (d == GameRulesNS::Difficulty::EASY) diffText = "Easy";
        else if (d == GameRulesNS::Difficulty::HARD) diffText = "Hard";

        _difficultyButton->setText(diffText);
    }

    if (_crossfireButton && _resourceManager->has<GameRules>()) {
        auto gameRules = _resourceManager->get<GameRules>();
        std::string crossfireText = gameRules->getCrossfire() ? "Yes" : "No";
        _crossfireButton->setText(crossfireText);
    }
}

void LobbyWaitingState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _lobbyCodeText.reset();
    _statusText.reset();
    _startGameButton.reset();
    _centerLayout.reset();
    _gamemodeLabel.reset();
    _gamemodeButton.reset();
    _difficultyLabel.reset();
    _difficultyButton.reset();
    _crossfireLabel.reset();
    _crossfireButton.reset();
    _topLeftLayout.reset();
    _chatButton.reset();
    _topRightLayout.reset();
    _loadingAnimation.reset();
    _bottomLeftLayout.reset();
    _copyCodeButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
