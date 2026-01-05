/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelCompleteState
*/

#include "LevelCompleteState.hpp"
#include <memory>
#include <string>
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../InGame/InGameState.hpp"
#include "../../../../../common/constants.hpp"

namespace gsm {

LevelCompleteState::LevelCompleteState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager), _waitingForNextLevel(true) {
    _uiManager = std::make_unique<ui::UIManager>();
}

void LevelCompleteState::enter() {
    std::string titleText = "LEVEL COMPLETE!";
    std::string subtitleText = "Going to the next level...";

    _titleText = std::make_shared<ui::Text>(_resourceManager);
    _titleText->setText(titleText);
    _titleText->setTextColor(colors::GREEN);
    _titleText->setFontSize(120);
    _titleText->setOutlineColor(gfx::color_t{0, 0, 0});
    _titleText->setOutlineThickness(2.0f);

    auto window = _resourceManager->get<gfx::IWindow>();
    auto titleSize = window->getTextSize(titleText, "assets/fonts/abduction2002.ttf", 120);
    float titleX = (constants::MAX_WIDTH - static_cast<float>(titleSize.first)) / 2.0f;
    float titleY = (
        constants::MAX_HEIGHT - static_cast<float>(titleSize.second)
    ) / 2.0f - 80.0f;
    _titleText->setPosition(math::Vector2f(titleX, titleY));

    _subtitleText = std::make_shared<ui::Text>(_resourceManager);
    _subtitleText->setText(subtitleText);
    _subtitleText->setTextColor(colors::WHITE);
    _subtitleText->setFontSize(40);
    _subtitleText->setOutlineColor(gfx::color_t{0, 0, 0});
    _subtitleText->setOutlineThickness(1.0f);

    auto subtitleSize = window->getTextSize(
        subtitleText, "assets/fonts/abduction2002.ttf", 40
    );
    float subtitleX = (constants::MAX_WIDTH - static_cast<float>(subtitleSize.first)) / 2.0f;
    float subtitleY = titleY + static_cast<float>(titleSize.second) + 40.0f;
    _subtitleText->setPosition(math::Vector2f(subtitleX, subtitleY));

    _uiManager->addElement(_titleText);
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
    _uiManager->render();

    window->display();
}

void LevelCompleteState::onNextLevel() {
    _waitingForNextLevel = false;
}

void LevelCompleteState::exit() {
    _uiManager.reset();
}

}  // namespace gsm
