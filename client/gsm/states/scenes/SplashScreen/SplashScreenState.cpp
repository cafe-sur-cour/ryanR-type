/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SplashScreenState Implementation
*/

#include "SplashScreenState.hpp"
#include <memory>
#include <string>
#include "../../../../colors.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../Connection/ConnectionState.hpp"

namespace gsm {

SplashScreenState::SplashScreenState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(constants::SPLASH_MAIN_TITLE_TEXT);
    _titleText->setFontSize(static_cast<unsigned int>(_startFontSize));
    _titleText->setTextColor(colors::RED);
    _titleText->setOutlineColor(colors::BLACK);
    _titleText->setOutlineThickness(constants::SPLASH_TITLE_OUTLINE_THICKNESS);

    _subtitleText = std::make_shared<ui::Text>(_resourceManager);
    _subtitleText->setText(constants::SPLASH_SUBTITLE_TEXT);
    _subtitleText->setFontSize(static_cast<unsigned int>(_startFontSize / 2.0f));
    _subtitleText->setTextColor(colors::YELLOW);
    _subtitleText->setOutlineColor(colors::BLACK);
    _subtitleText->setOutlineThickness(constants::SPLASH_SUBTITLE_OUTLINE_THICKNESS);

    auto window = _resourceManager->get<gfx::IWindow>();
    auto titleSize = window->getTextSize(
        constants::SPLASH_MAIN_TITLE_TEXT,
        constants::MAIN_FONT, static_cast<size_t>(_startFontSize));
    auto subtitleSize = window->getTextSize(
        constants::SPLASH_SUBTITLE_TEXT,
        constants::MAIN_FONT, static_cast<size_t>(_startFontSize / 2.0f));

    float totalHeight = static_cast<float>(titleSize.second) +
        static_cast<float>(subtitleSize.second) + constants::SPLASH_TEXT_PADDING;
    float startY = constants::WINDOW_HEIGHT / 2.0f - totalHeight / 2.0f;

    _titleText->setPosition(math::Vector2f(
        constants::WINDOW_WIDTH / 2.0f - static_cast<float>(titleSize.first) / 2.0f,
        startY
    ));

    _subtitleText->setPosition(math::Vector2f(
        constants::WINDOW_WIDTH / 2.0f - static_cast<float>(subtitleSize.first) / 2.0f,
        startY + static_cast<float>(titleSize.second) + 20.0f
    ));

    _uiManager->addElement(_titleText);
    _uiManager->addElement(_subtitleText);
}

void SplashScreenState::enter() {
    _animationTime = 0.0f;
    _currentFontSize = _startFontSize;

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        if (!audio->isMusicPlaying()) {
            float musicVolume = 100.0f;
            audio->setMusicVolume(musicVolume);
            audio->playMusic(constants::SPLASH_MUSIC_PATH, false);
        }
    }
}

void SplashScreenState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _animationTime += deltaTime;

    if (_animationTime < _growDuration) {
        _currentFontSize = _startFontSize + (_animationTime / _growDuration) *
            (_targetFontSize - _startFontSize);
        _titleText->setFontSize(static_cast<unsigned int>(_currentFontSize));

        float subtitleFontSize = _currentFontSize / 2.0f;
        _subtitleText->setFontSize(static_cast<unsigned int>(subtitleFontSize));

        auto window = _resourceManager->get<gfx::IWindow>();
        auto titleSize = window->getTextSize(
            constants::SPLASH_MAIN_TITLE_TEXT,
            constants::MAIN_FONT, static_cast<size_t>(_currentFontSize));
        auto subtitleSize = window->getTextSize(
            constants::SPLASH_SUBTITLE_TEXT,
            constants::MAIN_FONT, static_cast<size_t>(subtitleFontSize));

        float totalHeight = static_cast<float>(titleSize.second) +
        static_cast<float>(subtitleSize.second) + constants::SPLASH_TEXT_PADDING;
        float startY = constants::WINDOW_HEIGHT / 2.0f - totalHeight / 2.0f;

        _titleText->setPosition(math::Vector2f(
            constants::WINDOW_WIDTH / 2.0f - static_cast<float>(titleSize.first) / 2.0f,
            startY
        ));

        _subtitleText->setPosition(math::Vector2f(
            constants::WINDOW_WIDTH / 2.0f - static_cast<float>(subtitleSize.first) / 2.0f,
            startY + static_cast<float>(titleSize.second) + constants::SPLASH_TEXT_PADDING
        ));
    }

    if (_animationTime >= _totalDuration) {
        std::shared_ptr<gsm::ConnectionState> connectionState =
            std::make_shared<gsm::ConnectionState>(_gsm.lock(), _resourceManager);
        _gsm.lock()->requestStateChange(connectionState);
        return;
    }

    _uiManager->update(deltaTime);
    renderUI();
}

void SplashScreenState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        window->setCursor(false);
    }

    if (_uiManager) {
        _uiManager->clearElements();
    }
}

void SplashScreenState::renderUI() {
    if (_resourceManager->has<gfx::IWindow>()) {
        auto window = _resourceManager->get<gfx::IWindow>();
        _uiManager->render();
    }
}

}  // namespace gsm
