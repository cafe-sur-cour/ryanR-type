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
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

HowToPlayState::HowToPlayState(
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
    _titleText->setText("HOW TO PLAY");
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(gfx::color_t{100, 200, 255, 255});
    _titleText->setOutlineColor(gfx::color_t{20, 60, 100, 255});
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
    mainConfig.offset = math::Vector2f(0.0f, 20.0f);

    auto mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    mainLayout->setSize(math::Vector2f(1600.f, 450.f));

    ui::LayoutConfig controlsConfig;
    controlsConfig.direction = ui::LayoutDirection::Vertical;
    controlsConfig.alignment = ui::LayoutAlignment::Start;
    controlsConfig.spacing = 12.0f;
    controlsConfig.padding = math::Vector2f(30.0f, 25.0f);
    controlsConfig.anchorX = ui::AnchorX::Left;
    controlsConfig.anchorY = ui::AnchorY::Center;
    controlsConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto controlsSection = std::make_shared<ui::UILayout>(_resourceManager, controlsConfig);
    controlsSection->setSize(math::Vector2f(750.f, 450.f));

    auto controlsTitleBox = std::make_shared<ui::Box>(_resourceManager);
    controlsTitleBox->setSize(math::Vector2f(690.f, 60.f));
    controlsTitleBox->setBackgroundColor(gfx::color_t{50, 100, 150, 240});
    controlsTitleBox->setBorderColor(gfx::color_t{100, 180, 255, 255});
    controlsTitleBox->setBorderThickness(3.0f);

    ui::LayoutConfig controlsTitleConfig;
    controlsTitleConfig.direction = ui::LayoutDirection::Vertical;
    controlsTitleConfig.alignment = ui::LayoutAlignment::Center;
    controlsTitleConfig.spacing = 0.0f;
    controlsTitleConfig.padding = math::Vector2f(0.0f, 0.0f);
    controlsTitleConfig.anchorX = ui::AnchorX::Center;
    controlsTitleConfig.anchorY = ui::AnchorY::Center;

    auto controlsTitleLayout = std::make_shared<ui::UILayout>(_resourceManager,
        controlsTitleConfig);
    controlsTitleLayout->setSize(math::Vector2f(690.f, 60.f));

    auto controlsTitle = std::make_shared<ui::Text>(_resourceManager);
    controlsTitle->setText("CONTROLS");
    controlsTitle->setSize(math::Vector2f(650.f, 40.f));
    controlsTitle->setTextColor(gfx::color_t{255, 255, 255, 255});
    controlsTitle->setOutlineColor(gfx::color_t{50, 50, 50, 255});
    controlsTitle->setOutlineThickness(3.0f);

    controlsTitleLayout->addChild(controlsTitleBox);
    controlsTitleLayout->addElement(controlsTitle);
    controlsSection->addElement(controlsTitleLayout);

    std::vector<std::pair<std::string, gfx::color_t>> controls = {
        {"ARROWS : Move Ship", gfx::color_t{200, 230, 255, 255}},
        {"SPACE : Shoot", gfx::color_t{255, 200, 100, 255}},
        {"ESC : Menu", gfx::color_t{255, 150, 150, 255}}
    };

    for (const auto& [text, color] : controls) {
        auto controlBox = std::make_shared<ui::Box>(_resourceManager);
        controlBox->setSize(math::Vector2f(690.f, 55.f));
        controlBox->setBackgroundColor(gfx::color_t{30, 40, 60, 220});
        controlBox->setBorderColor(gfx::color_t{80, 120, 160, 200});
        controlBox->setBorderThickness(2.0f);

        ui::LayoutConfig controlConfig;
        controlConfig.direction = ui::LayoutDirection::Vertical;
        controlConfig.alignment = ui::LayoutAlignment::Center;
        controlConfig.spacing = 0.0f;
        controlConfig.padding = math::Vector2f(0.0f, 0.0f);
        controlConfig.anchorX = ui::AnchorX::Left;
        controlConfig.anchorY = ui::AnchorY::Center;

        auto controlLayout = std::make_shared<ui::UILayout>(_resourceManager, controlConfig);
        controlLayout->setSize(math::Vector2f(690.f, 55.f));

        auto controlText = std::make_shared<ui::Text>(_resourceManager);
        controlText->setText(text);
        controlText->setSize(math::Vector2f(650.f, 35.f));
        controlText->setTextColor(color);
        controlText->setOutlineColor(gfx::color_t{20, 20, 40, 255});
        controlText->setOutlineThickness(2.0f);

        controlLayout->addChild(controlBox);
        controlLayout->addElement(controlText);
        controlsSection->addElement(controlLayout);
    }

    ui::LayoutConfig objectivesConfig;
    objectivesConfig.direction = ui::LayoutDirection::Vertical;
    objectivesConfig.alignment = ui::LayoutAlignment::Start;
    objectivesConfig.spacing = 12.0f;
    objectivesConfig.padding = math::Vector2f(30.0f, 25.0f);
    objectivesConfig.anchorX = ui::AnchorX::Right;
    objectivesConfig.anchorY = ui::AnchorY::Center;
    objectivesConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto objectivesSection = std::make_shared<ui::UILayout>(_resourceManager,
        objectivesConfig);
    objectivesSection->setSize(math::Vector2f(750.f, 450.f));

    auto objectivesTitleBox = std::make_shared<ui::Box>(_resourceManager);
    objectivesTitleBox->setSize(math::Vector2f(690.f, 60.f));
    objectivesTitleBox->setBackgroundColor(gfx::color_t{100, 50, 150, 240});
    objectivesTitleBox->setBorderColor(gfx::color_t{180, 100, 255, 255});
    objectivesTitleBox->setBorderThickness(3.0f);

    ui::LayoutConfig objectivesTitleConfig;
    objectivesTitleConfig.direction = ui::LayoutDirection::Vertical;
    objectivesTitleConfig.alignment = ui::LayoutAlignment::Center;
    objectivesTitleConfig.spacing = 0.0f;
    objectivesTitleConfig.padding = math::Vector2f(0.0f, 0.0f);
    objectivesTitleConfig.anchorX = ui::AnchorX::Center;
    objectivesTitleConfig.anchorY = ui::AnchorY::Center;

    auto objectivesTitleLayout = std::make_shared<ui::UILayout>(_resourceManager,
        objectivesTitleConfig);
    objectivesTitleLayout->setSize(math::Vector2f(690.f, 60.f));

    auto objectivesTitle = std::make_shared<ui::Text>(_resourceManager);
    objectivesTitle->setText("OBJECTIVES");
    objectivesTitle->setSize(math::Vector2f(650.f, 40.f));
    objectivesTitle->setTextColor(gfx::color_t{255, 255, 255, 255});
    objectivesTitle->setOutlineColor(gfx::color_t{50, 50, 50, 255});
    objectivesTitle->setOutlineThickness(3.0f);

    objectivesTitleLayout->addChild(objectivesTitleBox);
    objectivesTitleLayout->addElement(objectivesTitle);
    objectivesSection->addElement(objectivesTitleLayout);

    std::vector<std::pair<std::string, gfx::color_t>> objectives = {
        {"> Destroy Enemy Ships", gfx::color_t{255, 100, 100, 255}},
        {"> Survive the Waves", gfx::color_t{100, 255, 200, 255}},
        {"> Collect Power-ups", gfx::color_t{255, 215, 0, 255}},
        {"> Beat High Score", gfx::color_t{255, 150, 255, 255}}
    };

    for (const auto& [text, color] : objectives) {
        auto objectiveBox = std::make_shared<ui::Box>(_resourceManager);
        objectiveBox->setSize(math::Vector2f(690.f, 55.f));
        objectiveBox->setBackgroundColor(gfx::color_t{40, 30, 60, 220});
        objectiveBox->setBorderColor(gfx::color_t{120, 80, 160, 200});
        objectiveBox->setBorderThickness(2.0f);

        ui::LayoutConfig objectiveConfig;
        objectiveConfig.direction = ui::LayoutDirection::Vertical;
        objectiveConfig.alignment = ui::LayoutAlignment::Center;
        objectiveConfig.spacing = 0.0f;
        objectiveConfig.padding = math::Vector2f(0.0f, 0.0f);
        objectiveConfig.anchorX = ui::AnchorX::Left;
        objectiveConfig.anchorY = ui::AnchorY::Center;

        auto objectiveLayout = std::make_shared<ui::UILayout>(_resourceManager,
            objectiveConfig);
        objectiveLayout->setSize(math::Vector2f(690.f, 55.f));

        auto objectiveText = std::make_shared<ui::Text>(_resourceManager);
        objectiveText->setText(text);
        objectiveText->setSize(math::Vector2f(650.f, 35.f));
        objectiveText->setTextColor(color);
        objectiveText->setOutlineColor(gfx::color_t{20, 20, 40, 255});
        objectiveText->setOutlineThickness(2.0f);

        objectiveLayout->addChild(objectiveBox);
        objectiveLayout->addElement(objectiveText);
        objectivesSection->addElement(objectiveLayout);
    }

    mainLayout->addElement(controlsSection);
    mainLayout->addElement(objectivesSection);
    _uiManager->addElement(mainLayout);

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
    _backButton->setText("BACK");
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

    bool isHoveringUI = _uiManager->isMouseHoveringAnyElement(mousePos);
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

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
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void HowToPlayState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
