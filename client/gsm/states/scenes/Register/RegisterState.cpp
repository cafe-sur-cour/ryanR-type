/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Register State
*/

#include "RegisterState.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../../../../libs/Multimedia/IWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

RegisterState::RegisterState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    auto config = _resourceManager->get<SettingsConfig>();
    if (config) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Vertical;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 30.0f;
    mainConfig.padding = math::Vector2f(0.0f, 0.0f);
    mainConfig.anchorX = ui::AnchorX::Center;
    mainConfig.anchorY = ui::AnchorY::Center;
    mainConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(400.f, 400.f));

    _usernameInput = std::make_shared<ui::TextInput>(_resourceManager);
    _usernameInput->setPlaceholder("Username");
    _usernameInput->setSize(math::Vector2f(400.f, 60.f));
    _usernameInput->setMaxLength(8);
    _usernameInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_usernameInput);
    });

    _passwordInput = std::make_shared<ui::TextInput>(_resourceManager);
    _passwordInput->setPlaceholder("Password");
    _passwordInput->setSize(math::Vector2f(400.f, 60.f));
    _passwordInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_passwordInput);
    });

    _confirmPasswordInput = std::make_shared<ui::TextInput>(_resourceManager);
    _confirmPasswordInput->setPlaceholder("Confirm Password");
    _confirmPasswordInput->setSize(math::Vector2f(400.f, 60.f));
    _confirmPasswordInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_confirmPasswordInput);
    });

    _registerButton = std::make_shared<ui::Button>(_resourceManager);
    _registerButton->setText("Register");
    _registerButton->setSize(math::Vector2f(400.f, 60.f));
    _registerButton->setOnRelease([this]() {
        std::string username = this->_usernameInput->getText();
        std::string password = this->_passwordInput->getText();
        std::string confirmPassword = this->_confirmPasswordInput->getText();

        if (username.empty() || password.empty() || confirmPassword.empty()) {
            return;
        }

        if (password != confirmPassword) {
            return;
        }

        const std::string filepath = "saves/users.json";
        nlohmann::json users;
        if (std::filesystem::exists(filepath)) {
            std::ifstream file(filepath);
            if (file.is_open()) {
                try {
                    file >> users;
                } catch (const std::exception& e) {
                    users = nlohmann::json::array();
                }
                file.close();
            }
        } else {
            users = nlohmann::json::array();
        }

        for (const auto& user : users) {
            if (user.contains("username") && user["username"] == username) {
                return;
            }
        }

        nlohmann::json newUser;
        newUser["username"] = username;
        newUser["password"] = password;
        users.push_back(newUser);

        std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << users.dump(4);
            file.close();
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePop();
            }
        }
    });
    _registerButton->setOnActivated([this]() {
        std::string username = this->_usernameInput->getText();
        std::string password = this->_passwordInput->getText();
        std::string confirmPassword = this->_confirmPasswordInput->getText();

        if (username.empty() || password.empty() || confirmPassword.empty()) {
            return;
        }

        if (password != confirmPassword) {
            return;
        }

        const std::string filepath = "saves/users.json";
        nlohmann::json users;
        if (std::filesystem::exists(filepath)) {
            std::ifstream file(filepath);
            if (file.is_open()) {
                try {
                    file >> users;
                } catch (const std::exception& e) {
                    users = nlohmann::json::array();
                }
                file.close();
            }
        } else {
            users = nlohmann::json::array();
        }

        for (const auto& user : users) {
            if (user.contains("username") && user["username"] == username) {
                return;
            }
        }

        nlohmann::json newUser;
        newUser["username"] = username;
        newUser["password"] = password;
        users.push_back(newUser);

        std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << users.dump(4);
            file.close();
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePop();
            }
        }
    });

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText("Back");
    _backButton->setSize(math::Vector2f(400.f, 60.f));
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });
    _backButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _mainLayout->addElement(_usernameInput);
    _mainLayout->addElement(_passwordInput);
    _mainLayout->addElement(_confirmPasswordInput);
    _mainLayout->addElement(_registerButton);
    _mainLayout->addElement(_backButton);

    _uiManager->addElement(_mainLayout);
}

void RegisterState::enter() {
}

void RegisterState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (config && _uiManager->getGlobalScale() != config->getUIScale()) {
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
    renderUI();
}

void RegisterState::renderUI() {
    _uiManager->render();
}

void RegisterState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }
}

}  // namespace gsm
