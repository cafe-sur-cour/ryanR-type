/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#include "ResultsState.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../../common/constants.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/debug.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../MainMenu/MainMenuState.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../ClientNetwork.hpp"

namespace gsm {

ResultsState::ResultsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isWin)
    : AGameState(gsm, resourceManager), _isWin(isWin) {
    _uiManager = std::make_unique<ui::UIManager>();
}

void ResultsState::enter() {
    updateUserStats();

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);

    auto window = _resourceManager->get<gfx::IWindow>();
    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    std::string text = _isWin ? constants::WIN_TEXT : constants::LOSE_TEXT;
    gfx::color_t color = _isWin ? colors::GREEN : colors::RED;

    _resultText = std::make_shared<ui::Text>(_resourceManager);
    _resultText->setText(text);
    _resultText->setTextColor(color);
    _resultText->setFontSize(150);
    _resultText->setOutlineColor(gfx::color_t{0, 0, 0});
    _resultText->setOutlineThickness(2.0f);

    auto textSize = window->getTextSize(text, constants::MAIN_FONT, 150);
    float x = (constants::MAX_WIDTH - static_cast<float>(textSize.first)) / 2.0f;
    float y = (constants::MAX_HEIGHT - static_cast<float>(textSize.second)) / 2.0f;
    _resultText->setPosition(math::Vector2f(x, y));

    _uiManager->addElement(_resultText);

    ui::LayoutConfig bottomRightConfig;
    bottomRightConfig.direction = ui::LayoutDirection::Vertical;
    bottomRightConfig.alignment = ui::LayoutAlignment::End;
    bottomRightConfig.spacing = 10.0f;
    bottomRightConfig.padding = math::Vector2f(0.0f, 0.0f);
    bottomRightConfig.anchorX = ui::AnchorX::Right;
    bottomRightConfig.anchorY = ui::AnchorY::Bottom;
    bottomRightConfig.offset = math::Vector2f(-20.0f, -20.0f);
}

void ResultsState::update(float deltaTime) {
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

    _uiManager->update(deltaTime);

    renderUI();
}

void ResultsState::renderUI() {
    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        auto windowSize = window->getWindowSize();
        window->drawFilledRectangle(
            gfx::color_t{0, 0, 0, 255},
            {0, 0},
            {static_cast<size_t>(windowSize.first), static_cast<size_t>(windowSize.second)}
        );
    }

    _uiManager->render();
}

void ResultsState::exit() {
    // TODO(anyone): Cleanup results state
}

void ResultsState::updateUserStats() {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config || config->getUsername().empty()) {
        return;
    }
}

}  // namespace gsm
