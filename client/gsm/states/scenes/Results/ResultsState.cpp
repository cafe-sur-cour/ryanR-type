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
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../../common/constants.hpp"
#include "../../../libs/Multimedia/IEvent.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../../../gsm/machine/AGameStateMachine.hpp"
#include "../MainMenu/MainMenuState.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

ResultsState::ResultsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isWin,
    int score)
    : AGameState(gsm, resourceManager), _isWin(isWin), _score(score) {
    _uiManager = std::make_unique<ui::UIManager>();
}

void ResultsState::enter() {
    updateUserStats();
    
    std::string text = _isWin ? constants::WIN_TEXT : constants::LOSE_TEXT;
    gfx::color_t color = _isWin ? colors::GREEN : colors::RED;

    _resultText = std::make_shared<ui::Text>(_resourceManager);
    _resultText->setText(text);
    _resultText->setTextColor(color);
    _resultText->setFontSize(150);
    _resultText->setOutlineColor(gfx::color_t{0, 0, 0});
    _resultText->setOutlineThickness(2.0f);

    auto window = _resourceManager->get<gfx::IWindow>();
    auto textSize = window->getTextSize(text, "assets/fonts/abduction2002.ttf", 150);
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

void ResultsState::updateUserStats() {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config || config->getUsername() == "Player") {
        return;
    }

    const std::string filepath = "saves/users.json";
    if (!std::filesystem::exists(filepath)) {
        std::cout << "No users file found!" << std::endl;
        return;
    }

    std::ifstream inFile(filepath);
    if (!inFile.is_open()) {
        std::cout << "Failed to read user data!" << std::endl;
        return;
    }

    nlohmann::json users;
    try {
        inFile >> users;
    } catch (const std::exception& e) {
        std::cout << "Failed to parse user data!" << std::endl;
        inFile.close();
        return;
    }
    inFile.close();

    std::string username = config->getUsername();
    bool userFound = false;
    
    for (auto& user : users) {
        if (user.contains("username") && user["username"] == username) {
            userFound = true;
            
            if (!user.contains("gamesPlayed")) user["gamesPlayed"] = 0;
            if (!user.contains("wins")) user["wins"] = 0;
            if (!user.contains("highScore")) user["highScore"] = 0;
            
            user["gamesPlayed"] = user["gamesPlayed"].get<int>() + 1;
            
            if (_isWin) {
                user["wins"] = user["wins"].get<int>() + 1;
            }
            
            if (_score > user["highScore"].get<int>()) {
                user["highScore"] = _score;
            }
            
            break;
        }
    }

    if (!userFound) {
        std::cout << "User not found in database!" << std::endl;
        return;
    }

    std::ofstream outFile(filepath);
    if (outFile.is_open()) {
        outFile << users.dump(4);
        outFile.close();
        std::cout << "User stats updated successfully!" << std::endl;
    } else {
        std::cout << "Failed to save user stats!" << std::endl;
    }
}

}  // namespace gsm
