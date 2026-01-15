/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef PROFILE_STATE_HPP_
#define PROFILE_STATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ClientNetwork.hpp"

namespace gsm {

class ReplayState;

class ProfileState : public AGameState {
public:
    ProfileState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~ProfileState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Profile"; }

private:
    void renderUI();
    void loadUserData();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::UILayout> _mainLayout;

    std::shared_ptr<ui::Text> _titleText;
    std::shared_ptr<ui::Text> _usernameText;
    std::shared_ptr<ui::Text> _gamesPlayedText;
    std::shared_ptr<ui::Text> _winsText;
    std::shared_ptr<ui::Text> _highScoreText;

    std::shared_ptr<ui::Button> _button1;
    std::shared_ptr<ui::Button> _button2;
    std::shared_ptr<ui::Button> _backButton;
};

}  // namespace gsm

#endif /* !PROFILE_STATE_HPP_ */
