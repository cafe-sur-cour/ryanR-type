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
#include <nlohmann/json.hpp>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../ui/elements/Box.hpp"

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
    titleConfig.offset = math::Vector2f(0.0f, 30.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(200.f, 250.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText("PLAYER PROFILE");
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(gfx::color_t{255, 215, 0, 255});
    _titleText->setOutlineColor(gfx::color_t{80, 60, 0, 255});
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Horizontal;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 50.0f;
    mainConfig.padding = math::Vector2f(30.0f, 30.0f);
    mainConfig.anchorX = ui::AnchorX::Center;
    mainConfig.anchorY = ui::AnchorY::Center;
    mainConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(1400.f, 400.f));

    // ========== SECTION STATISTIQUES ==========
    ui::LayoutConfig statsConfig;
    statsConfig.direction = ui::LayoutDirection::Vertical;
    statsConfig.alignment = ui::LayoutAlignment::Center;
    statsConfig.spacing = 15.0f;
    statsConfig.padding = math::Vector2f(25.0f, 20.0f);
    statsConfig.anchorX = ui::AnchorX::Left;
    statsConfig.anchorY = ui::AnchorY::Center;
    statsConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto statsSection = std::make_shared<ui::UILayout>(_resourceManager, statsConfig);
    statsSection->setSize(math::Vector2f(650.f, 350.f));

    // Box pour Username
    auto usernameBoxBackground = std::make_shared<ui::Box>(_resourceManager);
    usernameBoxBackground->setSize(math::Vector2f(600.f, 70.f));
    usernameBoxBackground->setBackgroundColor(gfx::color_t{40, 40, 70, 220});
    usernameBoxBackground->setBorderColor(gfx::color_t{100, 100, 140, 255});
    usernameBoxBackground->setBorderThickness(2.0f);

    ui::LayoutConfig usernameBoxConfig;
    usernameBoxConfig.direction = ui::LayoutDirection::Vertical;
    usernameBoxConfig.alignment = ui::LayoutAlignment::Center;
    usernameBoxConfig.spacing = 0.0f;
    usernameBoxConfig.padding = math::Vector2f(0.0f, 0.0f);
    usernameBoxConfig.anchorX = ui::AnchorX::Center;
    usernameBoxConfig.anchorY = ui::AnchorY::Center;
    usernameBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto usernameBox = std::make_shared<ui::UILayout>(_resourceManager, usernameBoxConfig);
    usernameBox->setSize(math::Vector2f(600.f, 70.f));

    _usernameText = std::make_shared<ui::Text>(_resourceManager);
    _usernameText->setText("Username: ");
    _usernameText->setSize(math::Vector2f(560.f, 50.f));
    _usernameText->setTextColor(gfx::color_t{255, 255, 100, 255});
    _usernameText->setOutlineColor(gfx::color_t{80, 80, 20, 255});
    _usernameText->setOutlineThickness(2.5f);

    usernameBox->addChild(usernameBoxBackground);
    usernameBox->addElement(_usernameText);

    // Box pour Games Played
    auto gamesBoxBackground = std::make_shared<ui::Box>(_resourceManager);
    gamesBoxBackground->setSize(math::Vector2f(600.f, 60.f));
    gamesBoxBackground->setBackgroundColor(gfx::color_t{40, 50, 70, 220});
    gamesBoxBackground->setBorderColor(gfx::color_t{100, 120, 160, 255});
    gamesBoxBackground->setBorderThickness(2.0f);

    ui::LayoutConfig gamesBoxConfig;
    gamesBoxConfig.direction = ui::LayoutDirection::Vertical;
    gamesBoxConfig.alignment = ui::LayoutAlignment::Center;
    gamesBoxConfig.spacing = 0.0f;
    gamesBoxConfig.padding = math::Vector2f(0.0f, 0.0f);
    gamesBoxConfig.anchorX = ui::AnchorX::Center;
    gamesBoxConfig.anchorY = ui::AnchorY::Center;
    gamesBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto gamesBox = std::make_shared<ui::UILayout>(_resourceManager, gamesBoxConfig);
    gamesBox->setSize(math::Vector2f(600.f, 60.f));

    _gamesPlayedText = std::make_shared<ui::Text>(_resourceManager);
    _gamesPlayedText->setText("Games Played: 0");
    _gamesPlayedText->setSize(math::Vector2f(560.f, 40.f));
    _gamesPlayedText->setTextColor(gfx::color_t{180, 220, 255, 255});
    _gamesPlayedText->setOutlineColor(gfx::color_t{40, 60, 80, 255});
    _gamesPlayedText->setOutlineThickness(2.0f);

    gamesBox->addChild(gamesBoxBackground);
    gamesBox->addElement(_gamesPlayedText);

    // Box pour Wins
    auto winsBoxBackground = std::make_shared<ui::Box>(_resourceManager);
    winsBoxBackground->setSize(math::Vector2f(600.f, 60.f));
    winsBoxBackground->setBackgroundColor(gfx::color_t{40, 70, 50, 220});
    winsBoxBackground->setBorderColor(gfx::color_t{80, 160, 100, 255});
    winsBoxBackground->setBorderThickness(2.0f);

    ui::LayoutConfig winsBoxConfig;
    winsBoxConfig.direction = ui::LayoutDirection::Vertical;
    winsBoxConfig.alignment = ui::LayoutAlignment::Center;
    winsBoxConfig.spacing = 0.0f;
    winsBoxConfig.padding = math::Vector2f(0.0f, 0.0f);
    winsBoxConfig.anchorX = ui::AnchorX::Center;
    winsBoxConfig.anchorY = ui::AnchorY::Center;
    winsBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto winsBox = std::make_shared<ui::UILayout>(_resourceManager, winsBoxConfig);
    winsBox->setSize(math::Vector2f(600.f, 60.f));

    _winsText = std::make_shared<ui::Text>(_resourceManager);
    _winsText->setText("Wins: 0");
    _winsText->setSize(math::Vector2f(560.f, 40.f));
    _winsText->setTextColor(gfx::color_t{100, 255, 150, 255});
    _winsText->setOutlineColor(gfx::color_t{20, 80, 40, 255});
    _winsText->setOutlineThickness(2.0f);

    winsBox->addChild(winsBoxBackground);
    winsBox->addElement(_winsText);

    // Box pour High Score
    auto scoreBoxBackground = std::make_shared<ui::Box>(_resourceManager);
    scoreBoxBackground->setSize(math::Vector2f(600.f, 70.f));
    scoreBoxBackground->setBackgroundColor(gfx::color_t{70, 60, 40, 220});
    scoreBoxBackground->setBorderColor(gfx::color_t{180, 140, 60, 255});
    scoreBoxBackground->setBorderThickness(2.0f);

    ui::LayoutConfig scoreBoxConfig;
    scoreBoxConfig.direction = ui::LayoutDirection::Vertical;
    scoreBoxConfig.alignment = ui::LayoutAlignment::Center;
    scoreBoxConfig.spacing = 0.0f;
    scoreBoxConfig.padding = math::Vector2f(0.0f, 0.0f);
    scoreBoxConfig.anchorX = ui::AnchorX::Center;
    scoreBoxConfig.anchorY = ui::AnchorY::Center;
    scoreBoxConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto scoreBox = std::make_shared<ui::UILayout>(_resourceManager, scoreBoxConfig);
    scoreBox->setSize(math::Vector2f(600.f, 70.f));

    _highScoreText = std::make_shared<ui::Text>(_resourceManager);
    _highScoreText->setText("High Score: 0");
    _highScoreText->setSize(math::Vector2f(560.f, 50.f));
    _highScoreText->setTextColor(gfx::color_t{255, 200, 50, 255});
    _highScoreText->setOutlineColor(gfx::color_t{100, 60, 0, 255});
    _highScoreText->setOutlineThickness(2.5f);

    scoreBox->addChild(scoreBoxBackground);
    scoreBox->addElement(_highScoreText);

    // Ajout des boxes de statistiques à la section stats
    statsSection->addElement(usernameBox);
    statsSection->addElement(gamesBox);
    statsSection->addElement(winsBox);
    statsSection->addElement(scoreBox);

    // ========== SECTION BOUTONS ==========
    ui::LayoutConfig buttonsConfig;
    buttonsConfig.direction = ui::LayoutDirection::Vertical;
    buttonsConfig.alignment = ui::LayoutAlignment::Center;
    buttonsConfig.spacing = 15.0f;
    buttonsConfig.padding = math::Vector2f(25.0f, 20.0f);
    buttonsConfig.anchorX = ui::AnchorX::Right;
    buttonsConfig.anchorY = ui::AnchorY::Center;
    buttonsConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto buttonsSection = std::make_shared<ui::UILayout>(_resourceManager, buttonsConfig);
    buttonsSection->setSize(math::Vector2f(500.f, 350.f));

    // Bouton 1 - À implémenter
    _button1 = std::make_shared<ui::Button>(_resourceManager);
    _button1->setText("Button 1");
    _button1->setSize(math::Vector2f(500.f, 60.f));
    _button1->setNormalColor(colors::BUTTON_SECONDARY);
    _button1->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _button1->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _button1->setOnRelease([]() {
    });

    // Bouton 2 - À implémenter
    _button2 = std::make_shared<ui::Button>(_resourceManager);
    _button2->setText("Button 2");
    _button2->setSize(math::Vector2f(500.f, 60.f));
    _button2->setNormalColor(colors::BUTTON_SECONDARY);
    _button2->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _button2->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _button2->setOnRelease([]() {
    });

    // Bouton 3 - À implémenter
    _button3 = std::make_shared<ui::Button>(_resourceManager);
    _button3->setText("Button 3");
    _button3->setSize(math::Vector2f(500.f, 60.f));
    _button3->setNormalColor(colors::BUTTON_SECONDARY);
    _button3->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _button3->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _button3->setOnRelease([]() {
    });

    buttonsSection->addElement(_button1);
    buttonsSection->addElement(_button2);
    buttonsSection->addElement(_button3);

    // Bouton Back
    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText("Back");
    _backButton->setSize(math::Vector2f(500.f, 70.f));
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

    // Ajout des sections au layout principal
    _mainLayout->addElement(statsSection);
    _mainLayout->addElement(buttonsSection);

    _uiManager->addElement(_mainLayout);

    // Layout séparé pour le bouton Back en bas
    ui::LayoutConfig backLayoutConfig;
    backLayoutConfig.direction = ui::LayoutDirection::Vertical;
    backLayoutConfig.alignment = ui::LayoutAlignment::Center;
    backLayoutConfig.spacing = 0.0f;
    backLayoutConfig.padding = math::Vector2f(0.0f, 0.0f);
    backLayoutConfig.anchorX = ui::AnchorX::Center;
    backLayoutConfig.anchorY = ui::AnchorY::Bottom;
    backLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto backLayout = std::make_shared<ui::UILayout>(_resourceManager, backLayoutConfig);
    backLayout->setSize(math::Vector2f(500.f, 70.f));
    backLayout->addElement(_backButton);

    _uiManager->addElement(backLayout);
}

void ProfileState::loadUserData() {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config) {
        return;
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
    _button1.reset();
    _button2.reset();
    _button3.reset();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void ProfileState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
