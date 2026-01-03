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
#include "../Settings/SettingsState.hpp"
#include "../LobbyWaiting/LobbyWaitingState.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../SettingsConfig.hpp"

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

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    auto net = this->_resourceManager->get<ClientNetwork>();
    _ipInput = std::make_shared<ui::TextInput>(_resourceManager);
    _ipInput->setPlaceholder(constants::IP_PLACEHOLDER);
    _ipInput->setText(net->getIp());
    _ipInput->setSize(math::Vector2f(300.f, 50.f));
    _ipInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_ipInput);
    });

    _portInput = std::make_shared<ui::TextInput>(_resourceManager);
    _portInput->setPlaceholder(constants::PORT_PLACEHOLDER);
    _portInput->setText(std::to_string(net->getPort()));
    _portInput->setSize(math::Vector2f(300.f, 50.f));
    _portInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_portInput);
    });

    ui::LayoutConfig leftConfig;
    leftConfig.direction = ui::LayoutDirection::Vertical;
    leftConfig.alignment = ui::LayoutAlignment::Center;
    leftConfig.spacing = 20.0f;
    leftConfig.padding = math::Vector2f(0.0f, 0.0f);
    leftConfig.anchorX = ui::AnchorX::Left;
    leftConfig.anchorY = ui::AnchorY::Center;
    leftConfig.offset = math::Vector2f(50.0f, 0.0f);

    _leftLayout = std::make_shared<ui::UILayout>(_resourceManager, leftConfig);
    _leftLayout->setSize(math::Vector2f(300.f, 300.f));

    _usernameDisplayText = std::make_shared<ui::Text>(_resourceManager);
    _usernameDisplayText->setText(config->getUsername());
    _usernameDisplayText->setSize(math::Vector2f(300.f, 40.f));
    _usernameDisplayText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _usernameDisplayText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _usernameDisplayText->setOutlineThickness(2.0f);
    _usernameDisplayText->setFontSize(28);

    _connectButton = std::make_shared<ui::Button>(_resourceManager);
    _connectButton->setText("Connect");
    _connectButton->setSize(math::Vector2f(300.f, 108.f));

    _connectButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            network->setIp(this->_ipInput->getText());
            try {
                int port = std::stoi(this->_portInput->getText());
                network->setPort(port);
                network->redoServerEndpoint();
            } catch (const std::exception&) {
                network->setPort(4242);
            }
            if (!network->isConnected()) {
                network->connect();
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Connecting to server...",
                    debug::debugType::NETWORK,
                    debug::debugLevel::INFO);
            }
        }
    });
    _connectButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            network->setIp(this->_ipInput->getText());
            try {
                int port = std::stoi(this->_portInput->getText());
                network->setPort(port);
                network->redoServerEndpoint();
            } catch (const std::exception&) {
                network->setPort(4242);
            }
            if (!network->isConnected()) {
                network->connect();
                debug::Debug::printDebug(network->isDebugMode(),
                    "[MainMenu] Connecting to server...",
                    debug::debugType::NETWORK,
                    debug::debugLevel::INFO);
            }
        }
    });

    _connectionStatusText = std::make_shared<ui::Text>(_resourceManager);
    _connectionStatusText->setText("Not connected to server");
    _connectionStatusText->setSize(math::Vector2f(300.f, 30.f));
    _connectionStatusText->setTextColor(gfx::color_t{255, 100, 100, 255});
    _connectionStatusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _connectionStatusText->setOutlineThickness(2.0f);

    _serverStatusText = std::make_shared<ui::Text>(_resourceManager);
    _serverStatusText->setText("");
    _serverStatusText->setSize(math::Vector2f(300.f, 40.f));
    _serverStatusText->setTextColor(gfx::color_t{100, 150, 255, 255});
    _serverStatusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
    _serverStatusText->setOutlineThickness(1.0f);

    _requestCodeButton = std::make_shared<ui::Button>(_resourceManager);
    _requestCodeButton->setText("Request Code");
    _requestCodeButton->setSize(math::Vector2f(300.f, 50.f));
    _requestCodeButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->requestCode();
        } else {
            std::cout << "Cannot request code: Not connected to server" << std::endl;
        }
    });

    _lobbyCodeInput = std::make_shared<ui::TextInput>(_resourceManager);
    _lobbyCodeInput->setPlaceholder("Enter lobby code");
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

    _leftLayout->addElement(_usernameDisplayText);
    _leftLayout->addElement(_ipInput);
    _leftLayout->addElement(_portInput);
    _leftLayout->addElement(_connectButton);
    _leftLayout->addElement(_connectionStatusText);
    _leftLayout->addElement(_serverStatusText);

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
    _playButton = std::make_shared<ui::Button>(resourceManager);
    _playButton->setText("Not connected");
    _playButton->setSize(math::Vector2f(576.f, 108.f));

    // _playButton->setOnRelease([this]() {
    //     auto network = this->_resourceManager->get<ClientNetwork>();
    //     if (network && network->isConnected()) {
    //         network->sendReady();
    //         debug::Debug::printDebug(network->isDebugMode(),
    //             "[MainMenu] Sent ready signal to server.",
    //             debug::debugType::NETWORK,
    //             debug::debugLevel::INFO);
    //     } else {
    //         debug::Debug::printDebug(network ? network->isDebugMode() : false,
    //             "[MainMenu] Cannot send ready: Not connected to server.",
    //             debug::debugType::NETWORK,
    //             debug::debugLevel::WARNING);
    //     }
    // });
    // _playButton->setOnActivated([this]() {
    //     auto network = this->_resourceManager->get<ClientNetwork>();
    //     if (network && network->isConnected()) {
    //         network->sendReady();
    //         debug::Debug::printDebug(network->isDebugMode(),
    //             "[MainMenu] Sent ready signal to server.",
    //             debug::debugType::NETWORK,
    //             debug::debugLevel::INFO);
    //     } else {
    //         debug::Debug::printDebug(network ? network->isDebugMode() : false,
    //             "[MainMenu] Cannot send ready: Not connected to server.",
    //             debug::debugType::NETWORK,
    //             debug::debugLevel::WARNING);
    //     }
    // });

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
    _rightLayout->setSize(math::Vector2f(300.f, 200.f));

    _rightLayout->addElement(_requestCodeButton);
    _rightLayout->addElement(_lobbyCodeInput);
    _rightLayout->addElement(_lobbyConnectButton);

    _uiManager->addElement(_leftLayout);
    _uiManager->addElement(_mainMenuLayout);
    _uiManager->addElement(_rightLayout);
}

void MainMenuState::enter() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (network) {
        _previousLobbyConnectedState = network->isConnectedToLobby();
        _previousLobbyMasterState = network->isLobbyMaster();
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

    bool isTextInputFocused = false;
    auto navManager = _uiManager->getNavigationManager();
    if (navManager) {
        auto focusedElement = navManager->getFocusedElement();
        if (focusedElement) {
            auto textInput = std::dynamic_pointer_cast<ui::TextInput>(focusedElement);
            isTextInputFocused = (textInput != nullptr);
        }
    }

    bool shouldBlockKeyboardInput = isTextInputFocused &&
        (eventResult == gfx::EventType::UP ||
         eventResult == gfx::EventType::DOWN ||
         eventResult == gfx::EventType::TAB);

    if (!shouldBlockKeyboardInput) {
        _uiManager->handleKeyboardInput(eventResult);
    }

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
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

    if (mousePressed && !isHoveringUI && navManager) {
        navManager->clearFocus();
    }

    if (_resourceManager->has<ecs::IInputProvider>() && !isTextInputFocused) {
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
    if (config && _usernameDisplayText) {
        _usernameDisplayText->setText(config->getUsername());
    }

    auto network = this->_resourceManager->get<ClientNetwork>();
    if (!network) {
        _playButton->setText("Not connected");
        _connectionStatusText->setText("Not connected");
        _connectionStatusText->setTextColor(gfx::color_t{255, 100, 100, 255});
        _connectionStatusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
        _connectionStatusText->setOutlineThickness(2.0f);
        _serverStatusText->setText("to server");
        return;
    }

    if (!network->getLobbyCode().empty()) {
        _lobbyCodeInput->setText(network->getLobbyCode());
        _requestCodeButton->setVisible(false);
    } else {
        _requestCodeButton->setVisible(true);
    }

    if (!network->isConnected()) {
        _playButton->setText("Not connected to server");
        _connectionStatusText->setText("Not connected to server");
        _connectionStatusText->setTextColor(gfx::color_t{255, 100, 100, 255});
        _connectionStatusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
        _connectionStatusText->setOutlineThickness(2.0f);
        _serverStatusText->setText("");
    } else {
        if (network->isReady()) {
            _playButton->setText("Waiting for players...");
        } else {
            _playButton->setText("Ready ?");
        }

        _connectionStatusText->setText("Connected");
        _connectionStatusText->setTextColor(gfx::color_t{100, 255, 100, 255});
        _connectionStatusText->setOutlineColor(gfx::color_t{0, 0, 0, 255});
        _connectionStatusText->setOutlineThickness(2.0f);

        std::string status = std::to_string(network->getConnectedClients()) + " players, ";
        status += std::to_string(network->getReadyClients()) + " ready";
        if (network->getClientId() != 0) {
            status += "\nYou are player " + std::to_string(network->getClientId());
            status += network->getClientReadyStatus() ? " (ready)" : " (not ready)";
        }
        _serverStatusText->setText(status);
    }
}

void MainMenuState::checkLobbyConnectionTransition() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (!network) {
        return;
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
    window->setCursor(false);
    _uiManager->clearElements();
    _playButton.reset();
    _settingsButton.reset();
    _quitButton.reset();
    _connectButton.reset();
    _requestCodeButton.reset();
    _lobbyConnectButton.reset();
    _ipInput.reset();
    _portInput.reset();
    _lobbyCodeInput.reset();
    _connectionStatusText.reset();
    _serverStatusText.reset();
    _usernameDisplayText.reset();
    _mainMenuLayout.reset();
    _rightLayout.reset();
    _leftLayout.reset();
    _background.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
