/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ForceLeaveState
*/

#include "ForceLeaveState.hpp"
#include <memory>
#include <string>
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

ForceLeaveState::ForceLeaveState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    constants::ForceLeaveType leaveType)
    : AGameState(gsm, resourceManager), _leaveType(leaveType) {
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);
}

void ForceLeaveState::enter() {
    auto networkPtr = this->_resourceManager->get<ClientNetwork>();
    if (networkPtr) {
        networkPtr->clearEntitiesAndMappings();
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);

    auto window = _resourceManager->get<gfx::IWindow>();
    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    std::string text;
    switch (_leaveType) {
        case constants::ForceLeaveType::CLOSED:
            text = "Lobby Closed";
            break;
        case constants::ForceLeaveType::KICKED:
            text = "Kicked";
            break;
        case constants::ForceLeaveType::BANNED:
            text = "Banned";
            break;
        default:
            text = "Unknown Reason";
            break;
    }

    _reasonText = std::make_shared<ui::Text>(_resourceManager);
    _reasonText->setText(text);
    _reasonText->setTextColor(colors::RED);
    _reasonText->setFontSize(150);
    _reasonText->setOutlineColor(gfx::color_t{0, 0, 0});
    _reasonText->setOutlineThickness(2.0f);

    auto textSize = window->getTextSize(text, constants::MAIN_FONT, 150);
    float x = (constants::MAX_WIDTH - static_cast<float>(textSize.first)) / 2.0f;
    float y = (constants::MAX_HEIGHT - static_cast<float>(textSize.second)) / 2.0f;
    _reasonText->setPosition(math::Vector2f(x, y));

    _uiManager->addElement(_reasonText);

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
        if (network) {
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
    });

    _leaveButton->setOnActivated([this]() {
        auto network = this->_resourceManager->get<ClientNetwork>();
        if (network) {
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
    });

    _bottomRightLayout->addElement(_leaveButton);
    _uiManager->addElement(_bottomRightLayout);
}

void ForceLeaveState::update(float deltaTime) {
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

void ForceLeaveState::renderUI() {
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

void ForceLeaveState::exit() {
    _reasonText.reset();
    _leaveButton.reset();
    _bottomRightLayout.reset();
}

}  // namespace gsm
