/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Leaderboard State
*/

#ifndef LEADERBOARDSTATE_HPP_
#define LEADERBOARDSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/core/UILayout.hpp"

namespace gsm {

class LeaderboardState : public AGameState {
private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;
    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::UILayout> _mainLayout;
    std::shared_ptr<ui::Text> _titleText;
    std::vector<std::shared_ptr<ui::Text>> _leaderTexts;
    std::shared_ptr<ui::Button> _backButton;

public:
    LeaderboardState(
        std::shared_ptr<IGameStateMachine> gsm,
        std::shared_ptr<ResourceManager> resourceManager
    );

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Leaderboard"; }

private:
    void loadLeaderboardData();
    void renderUI();
};

}  // namespace gsm

#endif /* !LEADERBOARDSTATE_HPP_ */
