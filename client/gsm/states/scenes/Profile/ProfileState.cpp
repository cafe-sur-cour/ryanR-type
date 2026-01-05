/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Profile State
*/

#include "ProfileState.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

ProfileState::ProfileState(
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

    // Titre en haut au centre
    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(0.0f, 0.0f);
    titleConfig.anchorX = ui::AnchorX::Center;
    titleConfig.anchorY = ui::AnchorY::Top;
    titleConfig.offset = math::Vector2f(0.0f, 50.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(150.f, 187.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText("PROFILE");
    _titleText->setSize(math::Vector2f(1500.f, 187.f));
    _titleText->setTextColor(gfx::color_t{255, 255, 255, 255});
    _titleText->setOutlineColor(gfx::color_t{100, 100, 100, 255});
    _titleText->setOutlineThickness(4.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Vertical;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 25.0f;
    mainConfig.padding = math::Vector2f(30.0f, 30.0f);
    mainConfig.anchorX = ui::AnchorX::Center;
    mainConfig.anchorY = ui::AnchorY::Center;
    mainConfig.offset = math::Vector2f(0.0f, 50.0f);

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(700.f, 600.f));

    // Box pour Username
    ui::LayoutConfig usernameBoxConfig;
    usernameBoxConfig.direction = ui::LayoutDirection::Vertical;
    usernameBoxConfig.alignment = ui::LayoutAlignment::Center;
    usernameBoxConfig.spacing = 0.0f;
    usernameBoxConfig.padding = math::Vector2f(20.0f, 15.0f);
    usernameBoxConfig.anchorX = ui::AnchorX::Center;
    usernameBoxConfig.anchorY = ui::AnchorY::Center;
    usernameBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto usernameBox = std::make_shared<ui::UILayout>(_resourceManager, usernameBoxConfig);
    usernameBox->setSize(math::Vector2f(650.f, 80.f));

    _usernameText = std::make_shared<ui::Text>(_resourceManager);
    _usernameText->setText("Username: ");
    _usernameText->setSize(math::Vector2f(610.f, 50.f));
    _usernameText->setTextColor(gfx::color_t{255, 255, 100, 255});
    _usernameText->setOutlineColor(gfx::color_t{80, 80, 20, 255});
    _usernameText->setOutlineThickness(2.5f);

    usernameBox->addElement(_usernameText);

    // Box pour Games Played
    ui::LayoutConfig gamesBoxConfig;
    gamesBoxConfig.direction = ui::LayoutDirection::Vertical;
    gamesBoxConfig.alignment = ui::LayoutAlignment::Center;
    gamesBoxConfig.spacing = 0.0f;
    gamesBoxConfig.padding = math::Vector2f(20.0f, 15.0f);
    gamesBoxConfig.anchorX = ui::AnchorX::Center;
    gamesBoxConfig.anchorY = ui::AnchorY::Center;
    gamesBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto gamesBox = std::make_shared<ui::UILayout>(_resourceManager, gamesBoxConfig);
    gamesBox->setSize(math::Vector2f(650.f, 70.f));

    _gamesPlayedText = std::make_shared<ui::Text>(_resourceManager);
    _gamesPlayedText->setText("Games Played: 0");
    _gamesPlayedText->setSize(math::Vector2f(610.f, 40.f));
    _gamesPlayedText->setTextColor(gfx::color_t{180, 220, 255, 255});
    _gamesPlayedText->setOutlineColor(gfx::color_t{40, 60, 80, 255});
    _gamesPlayedText->setOutlineThickness(2.0f);

    gamesBox->addElement(_gamesPlayedText);

    // Box pour Wins
    ui::LayoutConfig winsBoxConfig;
    winsBoxConfig.direction = ui::LayoutDirection::Vertical;
    winsBoxConfig.alignment = ui::LayoutAlignment::Center;
    winsBoxConfig.spacing = 0.0f;
    winsBoxConfig.padding = math::Vector2f(20.0f, 15.0f);
    winsBoxConfig.anchorX = ui::AnchorX::Center;
    winsBoxConfig.anchorY = ui::AnchorY::Center;
    winsBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto winsBox = std::make_shared<ui::UILayout>(_resourceManager, winsBoxConfig);
    winsBox->setSize(math::Vector2f(650.f, 70.f));

    _winsText = std::make_shared<ui::Text>(_resourceManager);
    _winsText->setText("Wins: 0");
    _winsText->setSize(math::Vector2f(610.f, 40.f));
    _winsText->setTextColor(gfx::color_t{100, 255, 150, 255});
    _winsText->setOutlineColor(gfx::color_t{20, 80, 40, 255});
    _winsText->setOutlineThickness(2.0f);

    winsBox->addElement(_winsText);

    // Box pour High Score
    ui::LayoutConfig scoreBoxConfig;
    scoreBoxConfig.direction = ui::LayoutDirection::Vertical;
    scoreBoxConfig.alignment = ui::LayoutAlignment::Center;
    scoreBoxConfig.spacing = 0.0f;
    scoreBoxConfig.padding = math::Vector2f(20.0f, 15.0f);
    scoreBoxConfig.anchorX = ui::AnchorX::Center;
    scoreBoxConfig.anchorY = ui::AnchorY::Center;
    scoreBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto scoreBox = std::make_shared<ui::UILayout>(_resourceManager, scoreBoxConfig);
    scoreBox->setSize(math::Vector2f(650.f, 80.f));

    _highScoreText = std::make_shared<ui::Text>(_resourceManager);
    _highScoreText->setText("High Score: 0");
    _highScoreText->setSize(math::Vector2f(610.f, 50.f));
    _highScoreText->setTextColor(gfx::color_t{255, 200, 50, 255});
    _highScoreText->setOutlineColor(gfx::color_t{100, 60, 0, 255});
    _highScoreText->setOutlineThickness(2.5f);

    scoreBox->addElement(_highScoreText);

    scoreBox->addElement(_highScoreText);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText("< Back");
    _backButton->setSize(math::Vector2f(500.f, 70.f));
    _backButton->setNormalColor(colors::BUTTON_PRIMARY);
    _backButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
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

    _mainLayout->addElement(usernameBox);
    _mainLayout->addElement(gamesBox);
    _mainLayout->addElement(winsBox);
    _mainLayout->addElement(scoreBox);
    _mainLayout->addElement(_backButton);

    _uiManager->addElement(_mainLayout);
}

void ProfileState::loadUserData() {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config) {
        return;
    }

    std::string username = config->getUsername();
    _usernameText->setText("Username: " + username);

    if (username == "Player") {
        _gamesPlayedText->setText("Games Played: 0");
        _winsText->setText("Wins: 0");
        _highScoreText->setText("High Score: 0");
        return;
    }

    const std::string filepath = "saves/users.json";
    if (!std::filesystem::exists(filepath)) {
        return;
    }

    std::ifstream file(filepath);
    if (!file.is_open()) {
        return;
    }

    nlohmann::json users;
    try {
        file >> users;
    } catch (const std::exception& e) {
        file.close();
        return;
    }
    file.close();

    if (!users.is_array()) {
        return;
    }

    for (const auto& user : users) {
        if (user.contains("username") && user["username"] == username) {
            int gamesPlayed = user.value("gamesPlayed", 0);
            int wins = user.value("wins", 0);
            int highScore = user.value("highScore", 0);

            _gamesPlayedText->setText("Games Played: " + std::to_string(gamesPlayed));
            _winsText->setText("Wins: " + std::to_string(wins));
            _highScoreText->setText("High Score: " + std::to_string(highScore));
            break;
        }
    }
}

void ProfileState::enter() {
    loadUserData();
}

void ProfileState::update(float deltaTime) {
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
    renderUI();
}

void ProfileState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }

    // Libération explicite des ressources
    _background.reset();
    _mainLayout.reset();
    _titleText.reset();
    _usernameText.reset();
    _gamesPlayedText.reset();
    _winsText.reset();
    _highScoreText.reset();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void ProfileState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
