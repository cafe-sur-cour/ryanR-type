/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Connection Scene
*/

#include "ConnectionState.hpp"
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
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../MainMenu/MainMenuState.hpp"
#include "../LevelEditorSelector/LevelEditorSelectorState.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"

namespace gsm {

ConnectionState::ConnectionState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
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
        math::Vector2f(6487.0f, 3584.0f));
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


    _loadingAnimation = std::make_shared<ui::SpritePreview>(_resourceManager);
    if (_loadingAnimation->loadPrefab(constants::LOADING_PREFAVB)) {
        _loadingAnimation->setSize(math::Vector2f(185.0f, 320.0f));
    } else {
        std::cerr << "Failed to load loading animation prefab" << std::endl;
        _loadingAnimation.reset();
    }
    _connectButton = std::make_shared<ui::Button>(_resourceManager);
    _connectButton->setText("Connect");
    _connectButton->setSize(math::Vector2f(300.f, 108.f));

    _connectButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            std::string ip = _ipInput->getText();
            std::string portStr = _portInput->getText();
            try {
                int port = std::stoi(portStr);
                network->setIp(ip);
                network->setPort(port);
                network->connect();
                if (_loadingAnimation) {
                    _uiManager->addElement(_loadingAnimation);
                    auto window = _resourceManager->get<gfx::IWindow>();
                    if (window) {
                        auto [windowWidth, windowHeight] = window->getWindowSize();
                        _loadingAnimation->setPosition(math::Vector2f(
                            static_cast<float>(windowWidth) - 200.0f,
                            static_cast<float>(windowHeight) - 300.0f
                        ));
                    }
                }
            } catch ([[maybe_unused]] const std::exception& e) {
                std::cerr << "Invalid port: " << portStr << std::endl;
            }
        }
    });
    _connectButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
            std::string ip = _ipInput->getText();
            std::string portStr = _portInput->getText();
            try {
                int port = std::stoi(portStr);
                network->setIp(ip);
                network->setPort(port);
                network->connect();
                if (_loadingAnimation) {
                    _uiManager->addElement(_loadingAnimation);
                    auto window = _resourceManager->get<gfx::IWindow>();
                    if (window) {
                        auto [windowWidth, windowHeight] = window->getWindowSize();
                        _loadingAnimation->setPosition(math::Vector2f(
                            static_cast<float>(windowWidth) - 200.0f,
                            static_cast<float>(windowHeight) - 300.0f
                        ));
                    }
                }
            } catch ([[maybe_unused]] const std::exception& e) {
                std::cerr << "Invalid port: " << portStr << std::endl;
            }
        }
    });

    _levelEditorButton = std::make_shared<ui::Button>(_resourceManager);
    _levelEditorButton->setText("Level Editor");
    _levelEditorButton->setSize(math::Vector2f(300.f, 108.f));
    _levelEditorButton->setNormalColor(colors::BUTTON_SECONDARY);
    _levelEditorButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _levelEditorButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _levelEditorButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(
                std::make_shared<LevelEditorSelectorState>(stateMachine,
                this->_resourceManager));
        }
    });
    _levelEditorButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(
                std::make_shared<LevelEditorSelectorState>(stateMachine,
                this->_resourceManager));
        }
    });

    _quitButton = std::make_shared<ui::Button>(_resourceManager);
    _quitButton->setText("Quit");
    _quitButton->setSize(math::Vector2f(300.f, 108.f));
    _quitButton->setNormalColor(colors::BUTTON_SECONDARY);
    _quitButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _quitButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _quitButton->setOnRelease([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });
    _quitButton->setOnActivated([this]() {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
    });

    _spacer = std::make_shared<ui::Text>(_resourceManager);
    _spacer->setText("");
    _spacer->setSize(math::Vector2f(300.f, 20.f));


    _spacer = std::make_shared<ui::Text>(_resourceManager);
    _spacer->setText("");
    _spacer->setSize(math::Vector2f(300.f, 20.f));

    ui::LayoutConfig layoutConfig;
    layoutConfig.direction = ui::LayoutDirection::Vertical;
    layoutConfig.alignment = ui::LayoutAlignment::Center;
    layoutConfig.spacing = 20.0f;
    layoutConfig.padding = math::Vector2f(0.0f, 0.0f);
    layoutConfig.anchorX = ui::AnchorX::Center;
    layoutConfig.anchorY = ui::AnchorY::Center;
    layoutConfig.offset = math::Vector2f(0.0f, 0.0f);

    _layout = std::make_shared<ui::UILayout>(_resourceManager, layoutConfig);
    _layout->setSize(math::Vector2f(300.f, 300.f));
    _layout->setAutoResize(true);

    _layout->addElement(_ipInput);
    _layout->addElement(_portInput);
    _layout->addElement(_connectButton);
    _layout->addElement(_spacer);
    _layout->addElement(_levelEditorButton);
    _layout->addElement(_quitButton);


    _uiManager->addElement(_layout);
}

void ConnectionState::enter() {
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
}

void ConnectionState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (_uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    if (_loadingAnimation) {
        _loadingAnimation->update(deltaTime);
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _uiManager->handleKeyboardInput(eventResult);

    if (eventResult == gfx::EventType::TEXT_INPUT) {
        _uiManager->handleTextInput(_resourceManager->get<gfx::IEvent>()->getLastTextInput());
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
    updateUIStatus();
    renderUI();
}

void ConnectionState::renderUI() {
    _uiManager->render();
}

void ConnectionState::updateUIStatus() {
    auto network = this->_resourceManager->get<ClientNetwork>();
    if (!network) return;

    if (network->isConnected()) {
        if (!_wasConnected) {
            _wasConnected = true;
            if (_loadingAnimation) {
                _uiManager->removeElement(_loadingAnimation);
                _loadingAnimation.reset();
            }
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePush(
                    std::make_shared<MainMenuState>(stateMachine, this->_resourceManager));
            }
        }
    }
}

void ConnectionState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }
}

}  // namespace gsm
