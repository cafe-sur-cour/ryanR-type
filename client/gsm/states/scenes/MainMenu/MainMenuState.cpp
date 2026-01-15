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
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../Settings/SettingsState.hpp"
#include "../Replay/ReplayState.hpp"
#include "../LobbyWaiting/LobbyWaitingState.hpp"
#include "../Register/RegisterState.hpp"
#include "../Login/LoginState.hpp"
#include "../Profile/ProfileState.hpp"
#include "../HowToPlay/HowToPlayState.hpp"
#include "../Connection/ConnectionState.hpp"
#include "../Leaderboard/LeaderboardState.hpp"
#include "../Chat/ChatState.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"

namespace gsm {

MainMenuState::MainMenuState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager), _previousLobbyConnectedState(false),
    _previousLobbyMasterState(false) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());
    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);
    _requestCodeButton = std::make_shared<ui::Button>(_resourceManager);
    _requestCodeButton->setText("Request Code");
    _requestCodeButton->setSize(math::Vector2f(300.f, 50.f));
    _requestCodeButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->requestCode();
        }
    });
    _lobbyCodeInput = std::make_shared<ui::TextInput>(_resourceManager);
    _lobbyCodeInput->setPlaceholder("Enter lobby code");
    _lobbyCodeInput->setMaxLength(12);
    _lobbyCodeInput->setSize(math::Vector2f(300.f, 50.f));
    _lobbyCodeInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_lobbyCodeInput);
    });

    _lobbyConnectButton = std::make_shared<ui::Button>(_resourceManager);
    _lobbyConnectButton->setText("Connect to Lobby");
    _lobbyConnectButton->setSize(math::Vector2f(300.f, 50.f));

    _lobbyConnectButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            std::string code = this->_lobbyCodeInput->getText();
            if (!code.empty()) {
                if (code.length() != constants::LOBBY_CODE_LENGTH) {
                    debug::Debug::printDebug(network->isDebugMode(),
                        "[MainMenu] Cannot connect to lobby: Code must be "
                        + std::to_string(constants::LOBBY_CODE_LENGTH) + " characters long",
                        debug::debugType::NETWORK,
                        debug::debugLevel::WARNING);
                    return;
                }
                network->setLobbyCode(code);
                network->sendLobbyConnection(code);
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Connecting to lobby with code: " + code,
                    debug::debugType::NETWORK,
                    debug::debugLevel::INFO);
            } else {
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Cannot connect to lobby: Code is empty",
                    debug::debugType::NETWORK,
                    debug::debugLevel::WARNING);
            }
        }
    });

    _lobbyConnectButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            std::string code = this->_lobbyCodeInput->getText();
            if (!code.empty()) {
                network->setLobbyCode(code);
                network->sendLobbyConnection(code);
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Connecting to lobby with code: " + code,
                    debug::debugType::NETWORK,
                    debug::debugLevel::INFO);
            } else {
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Cannot connect to lobby: Code is empty",
                    debug::debugType::NETWORK,
                    debug::debugLevel::WARNING);
            }
        }
    });

    ui::LayoutConfig menuConfig;
    menuConfig.direction = ui::LayoutDirection::Vertical;
    menuConfig.alignment = ui::LayoutAlignment::Center;
    menuConfig.spacing = 54.0f;
    menuConfig.padding = math::Vector2f(0.0f, 0.0f);
    menuConfig.anchorX = ui::AnchorX::Center;
    menuConfig.anchorY = ui::AnchorY::Center;
    menuConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainMenuLayout = std::make_shared<ui::UILayout>(_resourceManager, menuConfig);
    _mainMenuLayout->setSize(math::Vector2f(576.f, 450.f));
    _mainMenuLayout->setAutoResize(true);
    _usernameButton = std::make_shared<ui::Button>(_resourceManager);
    _usernameButton->setSize(math::Vector2f(576.f, 108.f));
    _usernameButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<ProfileState>(stateMachine,
                this->_resourceManager));
        }
    });
    _usernameButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && !network->getName().empty()) {
            network->sendRequestProfilePacket();
        }
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<ProfileState>(stateMachine,
                this->_resourceManager));
        }
    });

    _settingsButton = std::make_shared<ui::Button>(resourceManager);
    _settingsButton->setText("Settings");
    _settingsButton->setSize(math::Vector2f(576.f, 108.f));
    _settingsButton->setNormalColor(colors::BUTTON_SECONDARY);
    _settingsButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _settingsButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _settingsButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<SettingsState>(stateMachine,
                this->_resourceManager));
        }
    });
    _settingsButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<SettingsState>(stateMachine,
                this->_resourceManager));
        }
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

    _mainMenuLayout->addElement(_usernameButton);
    _mainMenuLayout->addElement(_settingsButton);
    _mainMenuLayout->addElement(_quitButton);

    ui::LayoutConfig rightConfig;
    rightConfig.direction = ui::LayoutDirection::Vertical;
    rightConfig.alignment = ui::LayoutAlignment::Center;
    rightConfig.spacing = 20.0f;
    rightConfig.padding = math::Vector2f(0.0f, 0.0f);
    rightConfig.anchorX = ui::AnchorX::Right;
    rightConfig.anchorY = ui::AnchorY::Center;
    rightConfig.offset = math::Vector2f(-50.0f, 0.0f);

    _rightLayout = std::make_shared<ui::UILayout>(_resourceManager, rightConfig);
    _rightLayout->setSize(math::Vector2f(400.f, 236.f));
    _rightLayout->setAutoResize(true);

    _rightLayout->addElement(_requestCodeButton);
    _rightLayout->addElement(_lobbyCodeInput);
    _rightLayout->addElement(_lobbyConnectButton);

    ui::LayoutConfig topLeftConfig;
    topLeftConfig.direction = ui::LayoutDirection::Horizontal;
    topLeftConfig.alignment = ui::LayoutAlignment::Center;
    topLeftConfig.spacing = 20.0f;
    topLeftConfig.padding = math::Vector2f(0.0f, 0.0f);
    topLeftConfig.anchorX = ui::AnchorX::Left;
    topLeftConfig.anchorY = ui::AnchorY::Top;
    topLeftConfig.offset = math::Vector2f(20.0f, 20.0f);

    _topLeftLayout = std::make_shared<ui::UILayout>(_resourceManager, topLeftConfig);
    _topLeftLayout->setSize(math::Vector2f(320.f, 80.f));

    ui::LayoutConfig headerConfig;
    headerConfig.direction = ui::LayoutDirection::Horizontal;
    headerConfig.alignment = ui::LayoutAlignment::Center;
    headerConfig.spacing = 20.0f;
    headerConfig.padding = math::Vector2f(0.0f, 0.0f);
    headerConfig.anchorX = ui::AnchorX::Right;
    headerConfig.anchorY = ui::AnchorY::Top;
    headerConfig.offset = math::Vector2f(-20.0f, 20.0f);

    _headerLayout = std::make_shared<ui::UILayout>(_resourceManager, headerConfig);
    _headerLayout->setSize(math::Vector2f(80.f, 80.f));

    _howToPlayButton = std::make_shared<ui::Button>(_resourceManager);
    _howToPlayButton->setSize(math::Vector2f(80.f, 80.f));
    _howToPlayButton->setIconPath(constants::HOW_TO_PLAY_PATH);
    _howToPlayButton->setIconSize(math::Vector2f(500.f, 500.f));
    _howToPlayButton->setNormalColor(colors::BUTTON_SECONDARY);
    _howToPlayButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _howToPlayButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _howToPlayButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<HowToPlayState>(stateMachine,
                this->_resourceManager));
        }
    });
    _howToPlayButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_unique<HowToPlayState>(stateMachine,
                this->_resourceManager));
        }
    });

    _leaderboardButton = std::make_shared<ui::Button>(_resourceManager);
    _leaderboardButton->setSize(math::Vector2f(80.f, 80.f));
    _leaderboardButton->setIconPath(constants::LEADERBOARD_PATH);
    _leaderboardButton->setIconSize(math::Vector2f(500.f, 500.f));
    _leaderboardButton->setNormalColor(colors::BUTTON_SECONDARY);
    _leaderboardButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _leaderboardButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _leaderboardButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendRequestLeaderboardPacket();
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePush(std::make_shared<LeaderboardState>(stateMachine,
                    this->_resourceManager));
            }
        }
    });
    _leaderboardButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendRequestLeaderboardPacket();
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePush(std::make_shared<LeaderboardState>(stateMachine,
                    this->_resourceManager));
            }
        }
    });

    _registerButton = std::make_shared<ui::Button>(_resourceManager);
    _registerButton->setText("Register");
    _registerButton->setSize(math::Vector2f(150.f, 80.f));
    _registerButton->setNormalColor(colors::BUTTON_PRIMARY);
    _registerButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _registerButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _registerButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<RegisterState>(stateMachine,
                this->_resourceManager));
        }
    });
    _registerButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<RegisterState>(stateMachine,
                this->_resourceManager));
        }
    });

    _loginButton = std::make_shared<ui::Button>(_resourceManager);
    _loginButton->setText("Log In");
    _loginButton->setSize(math::Vector2f(150.f, 80.f));
    _loginButton->setNormalColor(colors::BUTTON_SECONDARY);
    _loginButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _loginButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _loginButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<LoginState>(stateMachine,
                this->_resourceManager));
        }
    });
    _loginButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<LoginState>(stateMachine,
                this->_resourceManager));
        }
    });

    _disconnectButton = std::make_shared<ui::Button>(_resourceManager);
    _disconnectButton->setText("Q");
    _disconnectButton->setSize(math::Vector2f(80.f, 80.f));
    _disconnectButton->setNormalColor(colors::BUTTON_PRIMARY);
    _disconnectButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _disconnectButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _disconnectButton->setOnRelease([this]() {
        auto settingsConfig = this->_resourceManager->get<SettingsConfig>();
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            network->sendLoginPacket("", "");
        }
        if (settingsConfig) {
            settingsConfig->setUsername("");
            settingsConfig->saveSettings();
        }
        if (network) {
            network->setName("");
            network->setLobbyCode("");
        }
    });
    _disconnectButton->setOnActivated([this]() {
        auto settingsConfig = this->_resourceManager->get<SettingsConfig>();
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            network->sendLoginPacket("", "");
        }
        if (settingsConfig) {
            settingsConfig->setUsername("");
            settingsConfig->saveSettings();
        }
        if (network) {
            network->setName("");
            network->setLobbyCode("");
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

    _headerLayout->addElement(_leaderboardButton);
    _headerLayout->addElement(_howToPlayButton);
    _headerLayout->addElement(_disconnectButton);

    _topLeftLayout->addElement(_registerButton);
    _topLeftLayout->addElement(_loginButton);
    _topLeftLayout->addElement(_chatButton);

    _uiManager->addElement(_mainMenuLayout);
    _uiManager->addElement(_rightLayout);
    _uiManager->addElement(_topLeftLayout);
    _uiManager->addElement(_headerLayout);

    updateUIStatus();
}

void MainMenuState::enter() {
    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        if (!audio->isMusicPlaying()) {
            float musicVolume = 100.0f;
            if (_resourceManager->has<SettingsConfig>()) {
                musicVolume = _resourceManager->get<SettingsConfig>()->getMusicVolume();
            }
            audio->setMusicVolume(musicVolume);
            audio->playMusic(constants::MENU_MUSIC_PATH, true);
        }
    }

    auto network = _resourceManager->get<ClientNetwork>();
    if (network) {
        _previousLobbyConnectedState = network->isConnectedToLobby();
        _previousLobbyMasterState = network->isLobbyMaster();

        if (network->getLobbyCode() == constants::LOBBY_LEAVE_MARKER) {
            network->setLobbyCode("");
        }
        network->_isConnectedToLobby = false;
        network->_isLobbyMaster = false;
        network->_ready = false;
        network->_lastLeaveLobbyTime = std::chrono::steady_clock::now();
    }
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

    _uiManager->handleKeyboardInput(eventResult);

    if (eventResult == gfx::EventType::TEXT_INPUT) {
        std::string textInput = _resourceManager->get<gfx::IEvent>()->getLastTextInput();
        if (!textInput.empty()) {
            _uiManager->handleTextInput(textInput);
        }
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _uiManager->handleMouseInput(mousePos, mousePressed);

    bool isHoveringUI = _uiManager->isMouseHoveringAnyElement(mousePos);

    auto navManager = _uiManager->getNavigationManager();
    if (mousePressed && !isHoveringUI && navManager) {
        navManager->clearFocus();
    }

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    checkLobbyConnectionTransition();
    updateUIStatus();
    renderUI();
}

void MainMenuState::renderUI() {
    _uiManager->render();
}

void MainMenuState::updateUIStatus() {
    auto config = _resourceManager->get<SettingsConfig>();

    auto network = this->_resourceManager->get<ClientNetwork>();
    bool isAuthenticated = network && !network->getName().empty();

    if (!isAuthenticated) {
        if (_usernameButton && _usernameButton->getState() != ui::UIState::Disabled) {
            _usernameButton->setState(ui::UIState::Disabled);
        }
        if (_requestCodeButton && _requestCodeButton->getState() != ui::UIState::Disabled) {
            _requestCodeButton->setState(ui::UIState::Disabled);
        }
        if (_lobbyConnectButton && _lobbyConnectButton->getState() != ui::UIState::Disabled) {
            _lobbyConnectButton->setState(ui::UIState::Disabled);
        }
        if (_chatButton && _chatButton->getState() != ui::UIState::Disabled) {
            _chatButton->setState(ui::UIState::Disabled);
        }
    } else {
        if (_usernameButton && _usernameButton->getState() == ui::UIState::Disabled) {
            _usernameButton->setState(ui::UIState::Normal);
        }
        if (_requestCodeButton && _requestCodeButton->getState() == ui::UIState::Disabled) {
            _requestCodeButton->setState(ui::UIState::Normal);
        }
        if (_lobbyConnectButton && _lobbyConnectButton->getState() == ui::UIState::Disabled) {
            _lobbyConnectButton->setState(ui::UIState::Normal);
        }
        if (_chatButton && _chatButton->getState() == ui::UIState::Disabled) {
            _chatButton->setState(ui::UIState::Normal);
        }
    }

    if (!network) {
        if (!config->getUsername().empty()) {
            _usernameButton->setText(config->getUsername());
        }
        return;
    }

    if (!network->getLobbyCode().empty()) {
        _lobbyCodeInput->setText(network->getLobbyCode());
        _requestCodeButton->setVisible(false);
    } else {
        _requestCodeButton->setVisible(true);
    }

    if (network->getName().empty()) {
        if (config->getUsername().empty()) {
            _usernameButton->setText("Not logged in");
        } else {
            _usernameButton->setText(config->getUsername() + " (not logged in)");
        }
    } else {
        _usernameButton->setText(network->getName());
    }
}

void MainMenuState::checkLobbyConnectionTransition() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (!network) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto timeSinceLeave = std::chrono::duration_cast<std::chrono::seconds>(
        now - network->_lastLeaveLobbyTime).count();
    if (timeSinceLeave < 1) {
        _previousLobbyConnectedState = network->isConnectedToLobby();
        _previousLobbyMasterState = network->isLobbyMaster();
        _requestCodeButton->setState(ui::UIState::Disabled);
        _lobbyConnectButton->setState(ui::UIState::Disabled);
        return;
    } else {
        if (_requestCodeButton->getState() == ui::UIState::Disabled) {
            _requestCodeButton->setState(ui::UIState::Normal);
        }
        if (_lobbyConnectButton->getState() == ui::UIState::Disabled) {
            _lobbyConnectButton->setState(ui::UIState::Normal);
        }
    }

    bool currentLobbyConnected = network->isConnectedToLobby();
    bool currentLobbyMaster = network->isLobbyMaster();

    if (!_previousLobbyMasterState && currentLobbyMaster &&
        !network->getLobbyCode().empty()) {
        if (auto stateMachine = _gsm.lock()) {
            auto lobbyWaitingState = std::make_shared<LobbyWaitingState>(
                stateMachine, _resourceManager, true);
            stateMachine->requestStateChange(lobbyWaitingState);
            return;
        }
    }

    if (!_previousLobbyConnectedState && currentLobbyConnected) {
        if (auto stateMachine = _gsm.lock()) {
            auto lobbyWaitingState = std::make_shared<LobbyWaitingState>(
                stateMachine, _resourceManager, currentLobbyMaster);
            stateMachine->requestStateChange(lobbyWaitingState);
        }
    }
    _previousLobbyConnectedState = currentLobbyConnected;
    _previousLobbyMasterState = currentLobbyMaster;
}

void MainMenuState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }
}

}  // namespace gsm
