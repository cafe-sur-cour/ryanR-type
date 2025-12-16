/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#include "ResultsState.hpp"
#include <memory>
#include <string>
#include "../../../common/constants.hpp"
#include "../../../libs/Multimedia/IEvent.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../MainMenu/MainMenuState.hpp"

namespace gsm {

ResultsState::ResultsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isWin)
    : AGameState(gsm, resourceManager), _isWin(isWin) {
    _uiManager = std::make_unique<ui::UIManager>();
}

void ResultsState::enter() {
    std::string text = _isWin ? constants::WIN_TEXT : constants::LOSE_TEXT;
    gfx::color_t color = _isWin ? colors::GREEN : colors::RED;

    _resultText = std::make_shared<ui::Text>(_resourceManager);
    _resultText->setText(text);
    _resultText->setTextColor(color);
    _resultText->setFontSize(150);
    _resultText->setOutlineColor(gfx::color_t{0, 0, 0});
    _resultText->setOutlineThickness(2.0f);

    auto window = _resourceManager->get<gfx::IWindow>();
    auto textSize = window->getTextSize(text, "assets/fonts/arial.ttf", 150);
    float x = (constants::MAX_WIDTH - static_cast<float>(textSize.first)) / 2.0f;
    float y = (constants::MAX_HEIGHT - static_cast<float>(textSize.second)) / 2.0f;
    _resultText->setPosition(math::Vector2f(x, y));

    _uiManager->addElement(_resultText);
}

void ResultsState::update(float deltaTime) {
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

void ResultsState::exit() {
    // TODO(anyone): Cleanup results state
}

}  // namespace gsm
