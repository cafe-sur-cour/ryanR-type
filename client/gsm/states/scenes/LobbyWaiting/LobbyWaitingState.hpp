/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LobbyWaitingState
*/

#ifndef LOBBYWAITINGSTATE_HPP_
#define LOBBYWAITINGSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/elements/SpritePreview.hpp"

namespace gsm {

class LobbyWaitingState : public AGameState {
public:
    LobbyWaitingState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager, bool isLobbyMaster);
    ~LobbyWaitingState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Waiting to start"; }

private:
    void renderUI();
    void updateUIStatus();
    void setupLobbyMasterUI();
    void setupPlayerUI();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;
    std::shared_ptr<ui::UILayout> _centerLayout;
    std::shared_ptr<ui::Text> _lobbyCodeText;
    std::shared_ptr<ui::Text> _statusText;
    std::shared_ptr<ui::Button> _startGameButton;

    std::shared_ptr<ui::UILayout> _topLeftLayout;
    std::shared_ptr<ui::Text> _gamemodeLabel;
    std::shared_ptr<ui::Button> _gamemodeButton;
    std::shared_ptr<ui::Text> _difficultyLabel;
    std::shared_ptr<ui::Button> _difficultyButton;
    std::shared_ptr<ui::Text> _crossfireLabel;
    std::shared_ptr<ui::Button> _crossfireButton;
    std::shared_ptr<ui::Button> _leaveButton;

    std::shared_ptr<ui::UILayout> _topRightLayout;
    std::shared_ptr<ui::Button> _chatButton;
    std::shared_ptr<ui::SpritePreview> _loadingAnimation;

    std::shared_ptr<ui::UILayout> _bottomLeftLayout;
    std::shared_ptr<ui::Button> _copyCodeButton;

    bool _isLobbyMaster;
};

}  // namespace gsm

#endif  // LOBBYWAITINGSTATE_HPP_
