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
#include "../../../../ui/elements/Image.hpp"
#include "../Replay/ReplayState.hpp"

namespace gsm {

ProfileState::ProfileState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

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
        math::Vector2f(6487.0f, 3584.0f));
    _uiManager->addElement(_background);

    ui::LayoutConfig titleConfig;
    titleConfig.direction = ui::LayoutDirection::Vertical;
    titleConfig.alignment = ui::LayoutAlignment::Center;
    titleConfig.spacing = 0.0f;
    titleConfig.padding = math::Vector2f(0.0f, 0.0f);
    titleConfig.anchorX = ui::AnchorX::Left;
    titleConfig.anchorY = ui::AnchorY::Top;
    titleConfig.offset = math::Vector2f(640.0f, 30.0f);

    auto titleLayout = std::make_shared<ui::UILayout>(_resourceManager, titleConfig);
    titleLayout->setSize(math::Vector2f(200.f, 250.f));

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(constants::PROFILE_TITLE_TEXT);
    _titleText->setSize(math::Vector2f(2000.f, 250.f));
    _titleText->setTextColor(colors::BUTTON_PRIMARY_HOVER);
    _titleText->setOutlineColor(gfx::color_t{120, 0, 0, 255});
    _titleText->setFontSize(56);
    _titleText->setOutlineThickness(6.0f);

    titleLayout->addElement(_titleText);
    _uiManager->addElement(titleLayout);

    auto usernameImage = std::make_shared<ui::Image>(_resourceManager);
    usernameImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
    usernameImage->setSize(math::Vector2f(600.f, 70.f));
    usernameImage->setPosition(math::Vector2f(100.0f, 300.0f));
    _uiManager->addElement(usernameImage);

    _usernameText = std::make_shared<ui::Text>(_resourceManager);
    _usernameText->setText(constants::USERNAME_LABEL);
    _usernameText->setSize(math::Vector2f(560.f, 50.f));
    _usernameText->setPosition(math::Vector2f(200.0f, 337.0f));
    _usernameText->setTextColor(gfx::color_t{255, 255, 100, 255});
    _usernameText->setOutlineColor(gfx::color_t{80, 80, 20, 255});
    _usernameText->setOutlineThickness(2.5f);
    _uiManager->addElement(_usernameText);

    auto gamesImage = std::make_shared<ui::Image>(_resourceManager);
    gamesImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
    gamesImage->setSize(math::Vector2f(600.f, 60.f));
    gamesImage->setPosition(math::Vector2f(100.0f, 420.0f));
    _uiManager->addElement(gamesImage);

    _gamesPlayedText = std::make_shared<ui::Text>(_resourceManager);
    _gamesPlayedText->setText(constants::GAMES_PLAYED_LABEL + "0");
    _gamesPlayedText->setSize(math::Vector2f(560.f, 40.f));
    _gamesPlayedText->setPosition(math::Vector2f(200.0f, 457.0f));
    _gamesPlayedText->setTextColor(gfx::color_t{200, 220, 255, 255});
    _gamesPlayedText->setOutlineColor(gfx::color_t{40, 60, 80, 255});
    _gamesPlayedText->setOutlineThickness(2.0f);
    _uiManager->addElement(_gamesPlayedText);

    auto winsImage = std::make_shared<ui::Image>(_resourceManager);
    winsImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
    winsImage->setSize(math::Vector2f(600.f, 60.f));
    winsImage->setPosition(math::Vector2f(1050.0f, 300.0f));
    _uiManager->addElement(winsImage);

    _winsText = std::make_shared<ui::Text>(_resourceManager);
    _winsText->setText(constants::WINS_LABEL + "0");
    _winsText->setSize(math::Vector2f(560.f, 40.f));
    _winsText->setPosition(math::Vector2f(1150.0f, 337.0f));
    _winsText->setTextColor(gfx::color_t{100, 255, 150, 255});
    _winsText->setOutlineColor(gfx::color_t{20, 80, 40, 255});
    _winsText->setOutlineThickness(2.0f);
    _uiManager->addElement(_winsText);

    auto scoreImage = std::make_shared<ui::Image>(_resourceManager);
    scoreImage->setTexturePath(constants::LEADERBOARD_PLACEHOLDER_PATH);
    scoreImage->setSize(math::Vector2f(600.f, 70.f));
    scoreImage->setPosition(math::Vector2f(1050.0f, 420.0f));
    _uiManager->addElement(scoreImage);

    _highScoreText = std::make_shared<ui::Text>(_resourceManager);
    _highScoreText->setText(constants::HIGH_SCORE_LABEL + "0");
    _highScoreText->setSize(math::Vector2f(560.f, 50.f));
    _highScoreText->setPosition(math::Vector2f(1150.0f, 457.0f));
    _highScoreText->setTextColor(gfx::color_t{255, 200, 50, 255});
    _highScoreText->setOutlineColor(gfx::color_t{100, 60, 0, 255});
    _highScoreText->setOutlineThickness(2.5f);
    _uiManager->addElement(_highScoreText);

    _button1 = std::make_shared<ui::Button>(_resourceManager);
    _button1->setText(constants::REFRESH_PROFILE_BUTTON_TEXT);
    _button1->setSize(math::Vector2f(500.f, 60.f));
    _button1->setPosition(math::Vector2f(300.0f, 600.0f));
    _button1->setNormalColor(colors::BUTTON_PRIMARY);
    _button1->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _button1->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _button1->setOnRelease([this]() {
        auto network = _resourceManager->get<ClientNetwork>();
        if (network && !network->getName().empty()) {
            network->sendRequestProfilePacket();
        }
    });
    _uiManager->addElement(_button1);

    _button2 = std::make_shared<ui::Button>(_resourceManager);
    _button2->setText(constants::REPLAY_BUTTON_TEXT);
    _button2->setSize(math::Vector2f(500.f, 60.f));
    _button2->setPosition(math::Vector2f(1000.0f, 600.0f));
    _button2->setNormalColor(colors::BUTTON_PRIMARY);
    _button2->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _button2->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
    _button2->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<ReplayState>(stateMachine,
                this->_resourceManager));
        }
    });
    _button2->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePush(std::make_shared<ReplayState>(stateMachine,
                this->_resourceManager));
        }
    });
    _uiManager->addElement(_button2);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setText(constants::BACK_BUTTON_TEXT);
    _backButton->setSize(math::Vector2f(500.f, 70.f));
    _backButton->setPosition(math::Vector2f(650.0f, 920.0f));
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
    _uiManager->addElement(_backButton);
}

void ProfileState::loadUserData() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (!network) {
        return;
    }

    if (network->isProfileDataUpdated()) {
        auto profileData = network->getProfileData();
        if (profileData.size() >= 5) {
            _usernameText->setText(constants::USERNAME_LABEL + profileData[0]);
            _winsText->setText(constants::WINS_LABEL + profileData[1]);
            _highScoreText->setText(constants::HIGH_SCORE_LABEL + profileData[2]);
            _gamesPlayedText->setText(constants::GAMES_PLAYED_LABEL + profileData[3]);
        }

        network->clearProfileDataUpdateFlag();
    }
}

void ProfileState::enter() {
    auto network = _resourceManager->get<ClientNetwork>();
    if (network && !network->getName().empty()) {
        network->sendRequestProfilePacket();
    }

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

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    auto network = _resourceManager->get<ClientNetwork>();
    if (network && network->isProfileDataUpdated()) {
        loadUserData();
        network->clearProfileDataUpdateFlag();
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

    _background.reset();
    _titleText.reset();
    _usernameText.reset();
    _gamesPlayedText.reset();
    _winsText.reset();
    _highScoreText.reset();
    _button1.reset();
    _button2.reset();
    _backButton.reset();
    _uiManager.reset();
    _mouseHandler.reset();
}

void ProfileState::renderUI() {
    _uiManager->render();
}

}  // namespace gsm
