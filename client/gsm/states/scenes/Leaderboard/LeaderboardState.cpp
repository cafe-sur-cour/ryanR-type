/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Leaderboard State
*/

#include "LeaderboardState.hpp"

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

LeaderboardState::LeaderboardState(
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
        math::Vector2f(5376.0f, 3584.0f));
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
    _titleText->setText(constants::LEADERBOARD_TITLE_TEXT);
    _titleText->setFontSize(56);
    _titleText->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    _titleText->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Horizontal;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 50.0f;
    mainConfig.padding = math::Vector2f(30.0f, 30.0f);
    mainConfig.anchorX = ui::AnchorX::Left;
    mainConfig.anchorY = ui::AnchorY::Top;
    mainConfig.offset = math::Vector2f(70.0f, 200.0f);

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(1600.f, 600.f));

    ui::LayoutConfig leftConfig;
    leftConfig.direction = ui::LayoutDirection::Vertical;
    leftConfig.alignment = ui::LayoutAlignment::Center;
    leftConfig.spacing = 60.0f;

    auto leftLayout = std::make_shared<ui::UILayout>(_resourceManager, leftConfig);
    leftLayout->setSize(math::Vector2f(750.f, 600.f));

    ui::LayoutConfig rightConfig;
    rightConfig.direction = ui::LayoutDirection::Vertical;
    rightConfig.alignment = ui::LayoutAlignment::Center;
    rightConfig.spacing = 60.0f;

    auto rightLayout = std::make_shared<ui::UILayout>(_resourceManager, rightConfig);
    rightLayout->setSize(math::Vector2f(750.f, 600.f));

    for (int i = 0; i < 10; ++i) {
        auto entryBox = std::make_shared<ui::Image>(_resourceManager);
        entryBox->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
        entryBox->setSize(math::Vector2f(750.f, 50.f));

        float xPos = (i < 5) ? 200.0f : 980.0f;
        float yPos = 200.0f + ((i % 5) * 130.0f);
        entryBox->setPosition(math::Vector2f(xPos, yPos));

        _uiManager->addElement(entryBox);

        auto rankText = std::make_shared<ui::Text>(_resourceManager);
        std::stringstream ss;
        ss << (i + 1) << ".";
        rankText->setText(ss.str());
        rankText->setSize(math::Vector2f(60.f, 30.f));
        rankText->setPosition(math::Vector2f(xPos + 90.0f, yPos + 40.0f));
        rankText->setTextColor(gfx::color_t{255, 255, 255, 255});
        rankText->setFontSize(24);
        _uiManager->addElement(rankText);

        auto nameText = std::make_shared<ui::Text>(_resourceManager);
        std::stringstream ss2;
        ss2 << constants::LEADERBOARD_DEFAULT_NAME_PREFIX << (i + 1);
        nameText->setText(ss2.str());
        nameText->setSize(math::Vector2f(330.f, 30.f));
        nameText->setPosition(math::Vector2f(xPos + 158.0f, yPos + 40.0f));
        nameText->setTextColor(gfx::color_t{255, 255, 100, 255});
        nameText->setFontSize(24);
        _uiManager->addElement(nameText);

        auto scoreText = std::make_shared<ui::Text>(_resourceManager);
        std::stringstream ss3;
        ss3 << ((10 - i) * 1000);
        scoreText->setText(ss3.str());
        scoreText->setSize(math::Vector2f(180.f, 30.f));
        scoreText->setPosition(math::Vector2f(xPos + 506.0f, yPos + 40.0f));
        scoreText->setTextColor(gfx::color_t{100, 255, 150, 255});
        scoreText->setFontSize(24);
        _uiManager->addElement(scoreText);

        _leaderTexts.push_back(rankText);
        _leaderTexts.push_back(nameText);
        _leaderTexts.push_back(scoreText);
    }

    _uiManager->addElement(_mainLayout);

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

void LeaderboardState::loadLeaderboardData() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (network) {
        auto data = network->getLeaderboardData();
        if (!data.empty()) {
            std::vector<std::pair<std::string, std::string>> leaderboard = data;
            size_t numEntries = (std::min)(leaderboard.size(), _leaderTexts.size() / 3);
            for (size_t i = 0; i < numEntries; ++i) {
                size_t textIndex = i * 3;
                std::stringstream ss;
                ss << (i + 1) << ".";
                _leaderTexts[textIndex]->setText(ss.str());
                _leaderTexts[textIndex + 1]->setText(leaderboard[i].first);
                _leaderTexts[textIndex + 2]->setText(leaderboard[i].second);
            }

            for (size_t i = numEntries; i < 10; ++i) {
                size_t textIndex = i * 3;
                std::stringstream ss;
                ss << (i + 1) << ".";
                _leaderTexts[textIndex]->setText(ss.str());
                _leaderTexts[textIndex + 1]->setText(constants::LEADERBOARD_EMPTY_NAME);
                _leaderTexts[textIndex + 2]->setText(constants::LEADERBOARD_DEFAULT_SCORE);
            }
            return;
        }
    }

    for (size_t i = 0; i < 10; ++i) {
        size_t textIndex = i * 3;
        std::stringstream ss;
        ss << (i + 1) << ".";
        _leaderTexts[textIndex]->setText(ss.str());
        _leaderTexts[textIndex + 1]->setText(constants::LEADERBOARD_EMPTY_NAME);
        _leaderTexts[textIndex + 2]->setText(constants::LEADERBOARD_DEFAULT_SCORE);
    }
}

void LeaderboardState::enter() {
    loadLeaderboardData();
}

void LeaderboardState::update(float deltaTime) {
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
        loadLeaderboardData();
        network->clearLeaderboardDataUpdateFlag();
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void LeaderboardState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }

    _background.reset();
    _mainLayout.reset();
    _titleText.reset();
    for (auto& text : _leaderTexts) {
        text.reset();
    }
    _leaderTexts.clear();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void LeaderboardState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
