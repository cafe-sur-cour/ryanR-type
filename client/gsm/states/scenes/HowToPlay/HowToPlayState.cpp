/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** How to Play state
*/

#include "HowToPlayState.hpp"
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../../common/InputMapping/InputMappingManager.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../ui/elements/Image.hpp"

namespace gsm {

std::string getControlDisplayName(gfx::EventType eventType) {
    if (eventType >= gfx::EventType::UP && eventType <= gfx::EventType::NUMPAD_ENTER) {
        return ecs::InputMappingManager::eventTypeToString(eventType);
    }
    return "Unknown";
}

HowToPlayState::HowToPlayState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
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

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(0.0f, 0.0f);
    titleConfig.anchorX = ui::AnchorX::Left;
    titleConfig.anchorY = ui::AnchorY::Top;
    titleConfig.offset = math::Vector2f(650.0f, 30.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(200.f, 250.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(constants::HOW_TO_PLAY_TITLE_TEXT);
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    _titleText->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    _titleText->setFontSize(56);
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    auto controlsTitle = std::make_shared<ui::Text>(_resourceManager);
    controlsTitle->setText(constants::CONTROLS_TITLE_TEXT);
    controlsTitle->setSize(math::Vector2f(650.f, 40.f));
    controlsTitle->setPosition(math::Vector2f(430.0f, 250.0f));
    controlsTitle->setTextColor(colors::BUTTON_SECONDARY_HOVER);
    controlsTitle->setOutlineColor(gfx::color_t{120, 87, 0, 255});
    controlsTitle->setOutlineThickness(3.0f);
    controlsTitle->setFontSize(32);
    _uiManager->addElement(controlsTitle);

    auto inputMappingManager = _resourceManager->get<ecs::InputMappingManager>();
    std::vector<std::string> controls;

    if (inputMappingManager) {
        auto leftKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::MOVE_LEFT, true);
        auto rightKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::MOVE_RIGHT, true);
        auto upKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::MOVE_UP, true);
        auto downKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::MOVE_DOWN, true);

        std::string keyboardMove = getControlDisplayName(leftKey) + "/" +
            getControlDisplayName(upKey) + "/" +
            getControlDisplayName(downKey) + "/" + getControlDisplayName(rightKey);

        auto shootKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::SHOOT, true);
        auto forceKey = inputMappingManager->getKeyForRemappableAction(
            ecs::RemappableAction::FORCE, true);

        std::string gamepadShoot = "A";
        std::string gamepadForce = "X/Y";

        controls = {
            keyboardMove + " / Left Stick : Move Ship",
            getControlDisplayName(shootKey) + " / " + gamepadShoot + " : Shoot",
            getControlDisplayName(forceKey) + " / " + gamepadForce + " : Force",
            "ESC : Menu"
        };
    } else {
        controls = {
            "Z/Q/S/D / Left Stick : Move Ship",
            "SPACE / A : Shoot",
            "F / X/Y : Force",
            "ESC : Menu"
        };
    }

    float controlsXPos = 200.0f;
    float controlsStartY = 310.0f;
    for (size_t i = 0; i < controls.size(); ++i) {
        auto controlImage = std::make_shared<ui::Image>(_resourceManager);
        controlImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
        controlImage->setSize(math::Vector2f(750.f, 50.f));
        float yPos = controlsStartY + (static_cast<float>(i) * 120.0f);
        controlImage->setPosition(math::Vector2f(controlsXPos, yPos));
        _uiManager->addElement(controlImage);

        auto controlText = std::make_shared<ui::Text>(_resourceManager);
        controlText->setText(controls[i]);
        controlText->setSize(math::Vector2f(700.f, 30.f));
        controlText->setPosition(math::Vector2f(controlsXPos + 95.0f, yPos + 40.0f));
        controlText->setTextColor(gfx::color_t{255, 255, 255, 255});
        controlText->setFontSize(24);
        _uiManager->addElement(controlText);
        _controlTexts.push_back(controlText);
    }

    auto objectivesTitle = std::make_shared<ui::Text>(_resourceManager);
    objectivesTitle->setText(constants::OBJECTIVES_TITLE_TEXT);
    objectivesTitle->setSize(math::Vector2f(800.f, 40.f));
    objectivesTitle->setPosition(math::Vector2f(1220.0f, 250.0f));
    objectivesTitle->setTextColor(colors::BUTTON_SECONDARY_HOVER);
    objectivesTitle->setOutlineColor(gfx::color_t{120, 87, 0, 255});
    objectivesTitle->setOutlineThickness(3.0f);
    objectivesTitle->setFontSize(32);
    _uiManager->addElement(objectivesTitle);

    std::vector<std::string> objectives = {
        constants::OBJECTIVE_DESTROY_ENEMIES,
        constants::OBJECTIVE_SURVIVE_WAVES,
        constants::OBJECTIVE_COLLECT_POWERUPS,
        constants::OBJECTIVE_BEAT_HIGH_SCORE
    };

    float objectivesXPos = 980.0f;
    float objectivesStartY = 310.0f;
    for (size_t i = 0; i < objectives.size(); ++i) {
        auto objectiveImage = std::make_shared<ui::Image>(_resourceManager);
        objectiveImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
        objectiveImage->setSize(math::Vector2f(750.f, 50.f));
        float yPos = objectivesStartY + (static_cast<float>(i) * 120.0f);
        objectiveImage->setPosition(math::Vector2f(objectivesXPos, yPos));
        _uiManager->addElement(objectiveImage);

        auto objectiveText = std::make_shared<ui::Text>(_resourceManager);
        objectiveText->setText(objectives[i]);
        objectiveText->setSize(math::Vector2f(700.f, 30.f));
        objectiveText->setPosition(math::Vector2f(objectivesXPos + 95.0f, yPos + 40.0f));
        objectiveText->setTextColor(gfx::color_t{255, 255, 255, 255});
        objectiveText->setFontSize(24);
        _uiManager->addElement(objectiveText);
        _objectiveTexts.push_back(objectiveText);
    }

    ui::LayoutConfig backLayoutConfig;
    backLayoutConfig.direction = ui::LayoutDirection::Vertical;
    backLayoutConfig.alignment = ui::LayoutAlignment::Center;
    backLayoutConfig.spacing = 0.0f;
    backLayoutConfig.padding = math::Vector2f(0.0f, 0.0f);
    backLayoutConfig.anchorX = ui::AnchorX::Center;
    backLayoutConfig.anchorY = ui::AnchorY::Bottom;
    backLayoutConfig.offset = math::Vector2f(0.0f, -50.0f);

    auto backLayout = std::make_shared<ui::UILayout>(_resourceManager, backLayoutConfig);
    backLayout->setSize(math::Vector2f(400.f, 70.f));

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText(constants::BACK_BUTTON_TEXT_UPPER);
    _backButton->setSize(math::Vector2f(400.f, 70.f));
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

    backLayout->addElement(_backButton);
    _uiManager->addElement(backLayout);
}

void HowToPlayState::enter() {
}

void HowToPlayState::update(float deltaTime) {
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

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void HowToPlayState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }

    _background.reset();
    _titleText.reset();
    for (auto& text : _controlTexts) {
        text.reset();
    }
    _controlTexts.clear();
    for (auto& text : _objectiveTexts) {
        text.reset();
    }
    _objectiveTexts.clear();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void HowToPlayState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
