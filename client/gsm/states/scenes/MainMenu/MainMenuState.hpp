/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#ifndef MAINMENUSTATE_HPP_
#define MAINMENUSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/focusable/TextInput.hpp"
#include "../../../../ui/elements/Text.hpp"


namespace gsm {

class MainMenuState : public AGameState {
public:
    MainMenuState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~MainMenuState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();
    void updateUIStatus();
    void checkLobbyConnectionTransition();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::shared_ptr<ui::Button> _playButton;
    std::shared_ptr<ui::Button> _settingsButton;
    std::shared_ptr<ui::Button> _quitButton;
    std::shared_ptr<ui::Button> _connectButton;
    std::shared_ptr<ui::Button> _requestCodeButton;
    std::shared_ptr<ui::Button> _lobbyConnectButton;
    std::unique_ptr<ui::UIManager> _uiManager;
    std::shared_ptr<ui::UILayout> _leftLayout;
    std::shared_ptr<ui::UILayout> _mainMenuLayout;
    std::shared_ptr<ui::UILayout> _headerLayout;
    std::shared_ptr<ui::UILayout> _rightLayout;
    std::shared_ptr<ui::Button> _devButton;
    std::shared_ptr<ui::Button> _topRightButton;

    std::shared_ptr<ui::TextInput> _ipInput;
    std::shared_ptr<ui::TextInput> _portInput;
    std::shared_ptr<ui::TextInput> _lobbyCodeInput;

    std::shared_ptr<ui::Text> _connectionStatusText;
    std::shared_ptr<ui::Text> _serverStatusText;
    std::shared_ptr<ui::Text> _usernameDisplayText;

    std::shared_ptr<ui::Background> _background;

    bool _previousLobbyConnectedState;
    bool _previousLobbyMasterState;
};

}  // namespace gsm

#endif  // MAINMENUSTATE_HPP_
