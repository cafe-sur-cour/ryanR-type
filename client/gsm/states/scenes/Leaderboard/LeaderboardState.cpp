/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Leaderboard State
*/

#include "LeaderboardState.hpp"

#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>

#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/utils/SecureJsonManager.hpp"
#include "../../../../ui/elements/Box.hpp"

namespace gsm {

LeaderboardState::LeaderboardState(
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
    _titleText->setText("LEADERBOARD");
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(gfx::color_t{255, 215, 0, 255});
    _titleText->setOutlineColor(gfx::color_t{80, 60, 0, 255});
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    ui::LayoutConfig mainConfig;
    mainConfig.direction = ui::LayoutDirection::Vertical;
    mainConfig.alignment = ui::LayoutAlignment::Center;
    mainConfig.spacing = 10.0f;
    mainConfig.padding = math::Vector2f(30.0f, 30.0f);
    mainConfig.anchorX = ui::AnchorX::Center;
    mainConfig.anchorY = ui::AnchorY::Center;
    mainConfig.offset = math::Vector2f(0.0f, 0.0f);

    _mainLayout = std::make_shared<ui::UILayout>(_resourceManager, mainConfig);
    _mainLayout->setSize(math::Vector2f(800.f, 600.f));

    for (int i = 0; i < 10; ++i) {
        auto entryBox = std::make_shared<ui::Box>(_resourceManager);
        entryBox->setSize(math::Vector2f(750.f, 50.f));
        entryBox->setBackgroundColor(gfx::color_t{40, 40, 70, 220});
        entryBox->setBorderColor(gfx::color_t{100, 100, 140, 255});
        entryBox->setBorderThickness(2.0f);

        ui::LayoutConfig entryConfig;
        entryConfig.direction = ui::LayoutDirection::Horizontal;
        entryConfig.alignment = ui::LayoutAlignment::Center;
        entryConfig.spacing = 20.0f;
        entryConfig.padding = math::Vector2f(10.0f, 10.0f);
        entryConfig.anchorX = ui::AnchorX::Center;
        entryConfig.anchorY = ui::AnchorY::Center;
        entryConfig.offset = math::Vector2f(0.0f, 0.0f);

        auto entryLayout = std::make_shared<ui::UILayout>(_resourceManager, entryConfig);
        entryLayout->setSize(math::Vector2f(750.f, 50.f));

        auto rankText = std::make_shared<ui::Text>(_resourceManager);
        rankText->setText(std::to_string(i + 1) + ".");
        rankText->setSize(math::Vector2f(50.f, 30.f));
        rankText->setTextColor(gfx::color_t{255, 255, 255, 255});

        auto nameText = std::make_shared<ui::Text>(_resourceManager);
        nameText->setText("Player " + std::to_string(i + 1));
        nameText->setSize(math::Vector2f(400.f, 30.f));
        nameText->setTextColor(gfx::color_t{255, 255, 100, 255});

        auto scoreText = std::make_shared<ui::Text>(_resourceManager);
        scoreText->setText(std::to_string((10 - i) * 1000));
        scoreText->setSize(math::Vector2f(200.f, 30.f));
        scoreText->setTextColor(gfx::color_t{100, 255, 150, 255});

        entryLayout->addChild(entryBox);
        entryLayout->addElement(rankText);
        entryLayout->addElement(nameText);
        entryLayout->addElement(scoreText);

        _leaderTexts.push_back(rankText);
        _leaderTexts.push_back(nameText);
        _leaderTexts.push_back(scoreText);

        _mainLayout->addElement(entryLayout);
    }

    _uiManager->addElement(_mainLayout);

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
            size_t numEntries = std::min(leaderboard.size(), _leaderTexts.size() / 3);
            for (size_t i = 0; i < numEntries; ++i) {
                size_t textIndex = i * 3;
                _leaderTexts[textIndex]->setText(std::to_string(i + 1) + ".");
                _leaderTexts[textIndex + 1]->setText(leaderboard[i].first);
                _leaderTexts[textIndex + 2]->setText(leaderboard[i].second);
            }

            for (size_t i = numEntries; i < 10; ++i) {
                size_t textIndex = i * 3;
                _leaderTexts[textIndex]->setText(std::to_string(i + 1) + ".");
                _leaderTexts[textIndex + 1]->setText("---");
                _leaderTexts[textIndex + 2]->setText("0");
            }
            return;
        }
    }

    try {
        nlohmann::json usersData;
        std::ifstream file("saves/users.json");
        if (file.is_open()) {
            file >> usersData;
            file.close();
        } else {
            return;
        }
        if (!usersData.is_array()) {
            return;
        }

        std::vector<std::pair<std::string, std::string>> leaderboard;
        for (const auto& user : usersData) {
            if (user.contains("username") && user.contains("highScore")) {
                std::string username = user["username"];
                std::string highScore = std::to_string(user["highScore"].get<int>());
                leaderboard.emplace_back(username, highScore);
            }
        }

        std::sort(leaderboard.begin(), leaderboard.end(),
            [](const std::pair<std::string, std::string>& a,
                const std::pair<std::string, std::string>& b) {
                try {
                    return std::stoi(a.second) > std::stoi(b.second);
                } catch (const std::exception&) {
                    return false;
                }
            });

        size_t numEntries = std::min(leaderboard.size(), _leaderTexts.size() / 3);
        for (size_t i = 0; i < numEntries; ++i) {
            size_t textIndex = i * 3;
            _leaderTexts[textIndex]->setText(std::to_string(i + 1) + ".");
            _leaderTexts[textIndex + 1]->setText(leaderboard[i].first);
            _leaderTexts[textIndex + 2]->setText(leaderboard[i].second);
        }

        for (size_t i = numEntries; i < 10; ++i) {
            size_t textIndex = i * 3;
            _leaderTexts[textIndex]->setText(std::to_string(i + 1) + ".");
            _leaderTexts[textIndex + 1]->setText("---");
            _leaderTexts[textIndex + 2]->setText("0");
        }
    } catch (const std::exception&) {
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
