/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#include "LevelEditorState.hpp"
#include <optional>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"

namespace gsm {

LevelEditorState::LevelEditorState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    std::optional<std::filesystem::path> levelPath
) : AGameState(gsm, resourceManager), _levelPath(levelPath) {
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

    createUI();
}

void LevelEditorState::enter() {
    _uiManager->setOnBack([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
}

void LevelEditorState::update(float deltaTime) {
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
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void LevelEditorState::renderUI() {
    _uiManager->render();
}

void LevelEditorState::createUI() {
    // Side panel (left)
    ui::LayoutConfig sidePanelConfig;
    sidePanelConfig.direction = ui::LayoutDirection::Vertical;
    sidePanelConfig.alignment = ui::LayoutAlignment::Start;
    sidePanelConfig.spacing = 10.0f;
    sidePanelConfig.padding = math::Vector2f(10.0f, 10.0f);
    sidePanelConfig.anchorX = ui::AnchorX::Left;
    sidePanelConfig.anchorY = ui::AnchorY::Top;
    sidePanelConfig.offset = math::Vector2f(0.0f, 0.0f);
    sidePanelConfig.background.enabled = true;
    sidePanelConfig.background.fillColor = colors::PANEL_BACKGROUND;
    sidePanelConfig.background.outlineColor = colors::PANEL_BORDER;
    sidePanelConfig.background.cornerRadius = 0.0f; // Bords classiques

    _sidePanel = std::make_shared<ui::UILayout>(_resourceManager, sidePanelConfig);
    _sidePanel->setSize(math::Vector2f(300.0f, constants::MAX_HEIGHT - 200.0f)); // Laisser de la place pour le bottom panel
    _uiManager->addElement(_sidePanel);

    // Bottom panel
    ui::LayoutConfig bottomPanelConfig;
    bottomPanelConfig.direction = ui::LayoutDirection::Horizontal;
    bottomPanelConfig.alignment = ui::LayoutAlignment::Start;
    bottomPanelConfig.spacing = 10.0f;
    bottomPanelConfig.padding = math::Vector2f(10.0f, 10.0f);
    bottomPanelConfig.anchorX = ui::AnchorX::Left;
    bottomPanelConfig.anchorY = ui::AnchorY::Bottom;
    bottomPanelConfig.offset = math::Vector2f(0.0f, 0.0f);
    bottomPanelConfig.background.enabled = true;
    bottomPanelConfig.background.fillColor = colors::PANEL_BACKGROUND;
    bottomPanelConfig.background.outlineColor = colors::PANEL_BORDER;
    bottomPanelConfig.background.cornerRadius = 0.0f;

    _bottomPanel = std::make_shared<ui::UILayout>(_resourceManager, bottomPanelConfig);
    _bottomPanel->setSize(math::Vector2f(constants::MAX_WIDTH, 150.0f));
    _uiManager->addElement(_bottomPanel);

    // Canvas (right side)
    ui::LayoutConfig canvasConfig;
    canvasConfig.direction = ui::LayoutDirection::Vertical;
    canvasConfig.alignment = ui::LayoutAlignment::Center;
    canvasConfig.spacing = 0.0f;
    canvasConfig.padding = math::Vector2f(0.0f, 0.0f);
    canvasConfig.anchorX = ui::AnchorX::Right;
    canvasConfig.anchorY = ui::AnchorY::Top;
    canvasConfig.offset = math::Vector2f(0.0f, 0.0f);
    canvasConfig.background.enabled = true;
    canvasConfig.background.fillColor = colors::UI_BACKGROUND;
    canvasConfig.background.outlineColor = colors::PANEL_BORDER;
    canvasConfig.background.cornerRadius = 0.0f;

    _canvas = std::make_shared<ui::UILayout>(_resourceManager, canvasConfig);
    _canvas->setSize(math::Vector2f(constants::MAX_WIDTH - 300.0f, constants::MAX_HEIGHT - 150.0f)); // Ajuster pour le side et bottom panel
    _uiManager->addElement(_canvas);
}

void LevelEditorState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _background.reset();
    _sidePanel.reset();
    _bottomPanel.reset();
    _canvas.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
