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
    _uiManager->setResourceManager(_resourceManager);
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

    if (_isWin) {
        _victoryAnimation = std::make_shared<ui::SpritePreview>(_resourceManager);
        if (_victoryAnimation->loadPrefab(constants::VICTORY_PREFAB)) {
            _victoryAnimation->setSize(math::Vector2f(1285.0f, 724.0f));
            float x = (constants::MAX_WIDTH - 1285.0f) / 2.0f;
            float y = (constants::MAX_HEIGHT - 724.0f) / 2.0f;
            _victoryAnimation->setPosition(math::Vector2f(x, y));
            _uiManager->addElement(_victoryAnimation);
        } else {
            std::cerr << "Failed to load victory animation prefab" << std::endl;
            _victoryAnimation.reset();
        }
    } else {
        _youDiedAnimation = std::make_shared<ui::SpritePreview>(_resourceManager);
        if (_youDiedAnimation->loadPrefab(constants::LOSE_PREFAB)) {
            _youDiedAnimation->setSize(math::Vector2f(1080.0f, 235.0f));
            float x = (constants::MAX_WIDTH - 1080.0f) / 2.0f;
            float y = (constants::MAX_HEIGHT - 235.0f) / 2.0f;
            _youDiedAnimation->setPosition(math::Vector2f(x, y));
            _uiManager->addElement(_youDiedAnimation);
        } else {
            std::cerr << "Failed to load youdied animation prefab" << std::endl;
            _youDiedAnimation.reset();
        }
    }

    ui::LayoutConfig bottomRightConfig;
    bottomRightConfig.direction = ui::LayoutDirection::Vertical;
    bottomRightConfig.alignment = ui::LayoutAlignment::End;
    bottomRightConfig.spacing = 10.0f;
    bottomRightConfig.padding = math::Vector2f(0.0f, 0.0f);
    bottomRightConfig.anchorX = ui::AnchorX::Right;
    bottomRightConfig.anchorY = ui::AnchorY::Bottom;
    bottomRightConfig.offset = math::Vector2f(-20.0f, -20.0f);
    _bottomRightLayout =
        std::make_shared<ui::UILayout>(_resourceManager, bottomRightConfig);

    _leaveButton = std::make_shared<ui::Button>(_resourceManager);
    _leaveButton->setText("Leave");
    _leaveButton->setSize(math::Vector2f(150.f, 50.f));
    _leaveButton->setNormalColor(colors::BUTTON_DANGER);
    _leaveButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _leaveButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);

    _leaveButton->setOnRelease([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[ResultsState] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _leaveButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network && network->isConnected()) {
            network->sendDisconnectFromLobby();
            debug::Debug::printDebug(network->isDebugMode(),
                "[ResultsState] Player requested to leave the lobby.",
                debug::debugType::NETWORK,
                debug::debugLevel::INFO);
        }
    });

    _bottomRightLayout->addElement(_leaveButton);
    _uiManager->addElement(_bottomRightLayout);
}

void ResultsState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }
    auto network = this->_resourceManager->get<ClientNetwork>();
    if (network && network->_shouldDisconnect) {
        network->setLobbyCode("");
        network->_isConnectedToLobby = false;
        network->_isLobbyMaster = false;
        network->_ready = false;
        network->clearEntitiesAndMappings();
        auto gsmPtr = _gsm.lock();
        if (gsmPtr) {
            auto mainMenuState =
                std::make_shared<gsm::MainMenuState>(gsmPtr, _resourceManager);
            gsmPtr->requestStateChange(mainMenuState);
        }
        auto IAudio = this->_resourceManager->get<gfx::IAudio>();
        if (IAudio) {
            IAudio->stopAllSounds();
            IAudio->stopMusic();
        }
        network->_shouldDisconnect = false;
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

    if (_victoryAnimation) {
        _victoryAnimation->update(deltaTime);
    }
    if (_youDiedAnimation) {
        _youDiedAnimation->update(deltaTime);
    }

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
    _resultText.reset();
    _victoryAnimation.reset();
    _youDiedAnimation.reset();
    _leaveButton.reset();
    _bottomRightLayout.reset();
}

void ResultsState::updateUserStats() {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config || config->getUsername().empty()) {
        return;
    }
}

}  // namespace gsm
