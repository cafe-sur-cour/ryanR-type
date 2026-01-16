/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Leaderboard State
*/

#include "CreditsState.hpp"

#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/utils/SecureJsonManager.hpp"
#include "../../../../ui/elements/Image.hpp"

namespace gsm {

CreditsState::CreditsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    auto config = _resourceManager->get<SettingsConfig>();
    if (config) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(0.0f, 0.0f);
    titleConfig.anchorX = ui::AnchorX::Left;
    titleConfig.anchorY = ui::AnchorY::Top;
    titleConfig.offset = math::Vector2f(665.0f, 30.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(200.f, 250.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(constants::CREDITS_TITLE_TEXT);
    _titleText->setFontSize(56);
    _titleText->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    _titleText->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig leftColumnConfig;
    leftColumnConfig.direction = ui::LayoutDirection::Vertical;
    leftColumnConfig.alignment = ui::LayoutAlignment::Start;
    leftColumnConfig.spacing = 20.0f;
    leftColumnConfig.padding = math::Vector2f(30.0f, 30.0f);
    leftColumnConfig.anchorX = ui::AnchorX::Left;
    leftColumnConfig.anchorY = ui::AnchorY::Top;
    leftColumnConfig.offset = math::Vector2f(300.0f, 200.0f);

    auto leftLayout = std::make_shared<ui::UILayout>(_resourceManager, leftColumnConfig);
    leftLayout->setSize(math::Vector2f(600.f, 600.f));

    ui::LayoutConfig orgSectionConfig;
    orgSectionConfig.direction = ui::LayoutDirection::Vertical;
    orgSectionConfig.alignment = ui::LayoutAlignment::Start;
    orgSectionConfig.spacing = 40.0f;
    orgSectionConfig.padding = math::Vector2f(0.0f, 0.0f);
    orgSectionConfig.anchorX = ui::AnchorX::Left;
    orgSectionConfig.anchorY = ui::AnchorY::Top;
    orgSectionConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto orgSection = std::make_shared<ui::UILayout>(_resourceManager, orgSectionConfig);
    orgSection->setSize(math::Vector2f(600.f, 150.f));

    auto orgLabel = std::make_shared<ui::Text>(_resourceManager);
    orgLabel->setText("Organization");
    orgLabel->setFontSize(32);
    orgLabel->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    orgLabel->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    orgLabel->setOutlineThickness(3.0f);
    orgSection->addElement(orgLabel);

    auto orgName = std::make_shared<ui::Text>(_resourceManager);
    orgName->setText("Cafe sur Cour");
    orgName->setFontSize(48);
    orgName->setTextColor(gfx::color_t{255, 255, 100, 255});
    orgName->setOutlineColor(gfx::color_t{80, 80, 0, 255});
    orgName->setOutlineThickness(4.0f);
    orgSection->addElement(orgName);

    leftLayout->addElement(orgSection);

    ui::LayoutConfig studioSectionConfig;
    studioSectionConfig.direction = ui::LayoutDirection::Vertical;
    studioSectionConfig.alignment = ui::LayoutAlignment::Start;
    studioSectionConfig.spacing = 40.0f;
    studioSectionConfig.padding = math::Vector2f(0.0f, 0.0f);
    studioSectionConfig.anchorX = ui::AnchorX::Left;
    studioSectionConfig.anchorY = ui::AnchorY::Top;
    studioSectionConfig.offset = math::Vector2f(0.0f, 0.0f);

    auto studioSection = std::make_shared<ui::UILayout>(_resourceManager, studioSectionConfig);
    studioSection->setSize(math::Vector2f(600.f, 150.f));

    auto studioLabel = std::make_shared<ui::Text>(_resourceManager);
    studioLabel->setText("Studio");
    studioLabel->setFontSize(32);
    studioLabel->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    studioLabel->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    studioLabel->setOutlineThickness(3.0f);
    studioSection->addElement(studioLabel);

    auto studioName = std::make_shared<ui::Text>(_resourceManager);
    studioName->setText("RyanR-Type");
    studioName->setFontSize(48);
    studioName->setTextColor(gfx::color_t{255, 255, 100, 255});
    studioName->setOutlineColor(gfx::color_t{80, 80, 0, 255});
    studioName->setOutlineThickness(4.0f);
    studioSection->addElement(studioName);

    leftLayout->addElement(studioSection);

    _uiManager->addElement(leftLayout);

    ui::LayoutConfig rightColumnConfig;
    rightColumnConfig.direction = ui::LayoutDirection::Vertical;
    rightColumnConfig.alignment = ui::LayoutAlignment::Start;
    rightColumnConfig.spacing = 50.0f;
    rightColumnConfig.padding = math::Vector2f(30.0f, 30.0f);
    rightColumnConfig.anchorX = ui::AnchorX::Left;
    rightColumnConfig.anchorY = ui::AnchorY::Top;
    rightColumnConfig.offset = math::Vector2f(1100.0f, 200.0f);

    auto rightLayout = std::make_shared<ui::UILayout>(_resourceManager, rightColumnConfig);
    rightLayout->setSize(math::Vector2f(700.f, 600.f));

    auto devsLabel = std::make_shared<ui::Text>(_resourceManager);
    devsLabel->setText("Developers");
    devsLabel->setFontSize(32);
    devsLabel->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    devsLabel->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    devsLabel->setOutlineThickness(3.0f);
    rightLayout->addElement(devsLabel);

    for (std::size_t i = 0; i < constants::CREDITS_DEVS.size(); ++i) {
        auto nameText = std::make_shared<ui::Text>(_resourceManager);
        nameText->setText(constants::CREDITS_DEVS[i]);
        nameText->setTextColor(gfx::color_t{255, 255, 100, 255});
        nameText->setFontSize(28);
        rightLayout->addElement(nameText);
        _creditTexts.push_back(nameText);
    }

    _uiManager->addElement(rightLayout);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText(constants::BACK_BUTTON_TEXT);
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

void CreditsState::enter() {
}

void CreditsState::update(float deltaTime) {
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

    auto network = _resourceManager->get<ClientNetwork>();
    if (network && network->isLeaderboardDataUpdated()) {
        network->clearLeaderboardDataUpdateFlag();
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void CreditsState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }

    _background.reset();
    _titleText.reset();
    for (auto& text : _creditTexts) {
        text.reset();
    }
    _creditTexts.clear();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void CreditsState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
