/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelCompleteState
*/

#include "LevelCompleteState.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../InGame/InGameState.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

LevelCompleteState::LevelCompleteState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager), _waitingForNextLevel(true) {
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);
}

void LevelCompleteState::enter() {
    std::string subtitleText = constants::LEVEL_COMPLETE_SUB_TITLE_TEXT;

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->stopMusic();
        float musicVolume = 100.0f;
        if (_resourceManager->has<SettingsConfig>()) {
            musicVolume = _resourceManager->get<SettingsConfig>()->getMusicVolume();
        }
        audio->setMusicVolume(musicVolume);
        audio->playMusic(constants::VICTORY_MUSIC_PATH, true);
    }

    _victoryAnimation = std::make_shared<ui::SpritePreview>(_resourceManager);
    if (_victoryAnimation->loadPrefab(constants::VICTORY_PREFAB)) {
        _victoryAnimation->setSize(math::Vector2f(1285.0f, 724.0f));
        float animX = (constants::MAX_WIDTH - 1285.0f) / 2.0f;
        float animY = (constants::MAX_HEIGHT - 724.0f) / 2.0f - 80.0f;
        _victoryAnimation->setPosition(math::Vector2f(animX, animY));
    } else {
        std::cerr << "Failed to load victory animation prefab" << std::endl;
        _victoryAnimation.reset();
    }

    _subtitleText = std::make_shared<ui::Text>(_resourceManager);
    _subtitleText->setText(subtitleText);
    _subtitleText->setTextColor(colors::WHITE);
    _subtitleText->setFontSize(40);
    _subtitleText->setOutlineColor(colors::BLACK);
    _subtitleText->setOutlineThickness(1.0f);

    auto window = _resourceManager->get<gfx::IWindow>();
    auto subtitleSize = window->getTextSize(
        subtitleText, constants::MAIN_FONT, 40
    );
    float subtitleX = (constants::MAX_WIDTH - static_cast<float>(subtitleSize.first)) / 2.0f;
    float subtitleY = ((constants::MAX_HEIGHT - 724.0f) / 2.0f - 80.0f) + 724.0f + 40.0f;
    _subtitleText->setPosition(math::Vector2f(subtitleX, subtitleY));

    if (_victoryAnimation) {
        _uiManager->addElement(_victoryAnimation);
    }
    _uiManager->addElement(_subtitleText);

    _waitingForNextLevel = true;
}

void LevelCompleteState::update(float deltaTime) {
    auto event = _resourceManager->get<gfx::IEvent>();
    auto eventResult = event->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    auto window = _resourceManager->get<gfx::IWindow>();
    window->clear();

    window->updateView();

    _uiManager->update(deltaTime);
    if (_victoryAnimation) {
        _victoryAnimation->update(deltaTime);
    }
    _uiManager->render();

    window->display();
}

void LevelCompleteState::onNextLevel() {
    _waitingForNextLevel = false;
}

void LevelCompleteState::exit() {
    _victoryAnimation.reset();
    _uiManager.reset();
}

}  // namespace gsm
