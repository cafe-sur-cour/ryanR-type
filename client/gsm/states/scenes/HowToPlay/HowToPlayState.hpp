/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** How to Play State
*/

#ifndef HOW_TO_PLAY_HPP_
#define HOW_TO_PLAY_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/Box.hpp"

namespace gsm {

class HowToPlayState : public AGameState {
public:
    HowToPlayState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~HowToPlayState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "How to Play"; }

private:
    void renderUI();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Text> _titleText;
    std::shared_ptr<ui::Button> _backButton;
    std::vector<std::shared_ptr<ui::Text>> _controlTexts;
    std::vector<std::shared_ptr<ui::Text>> _objectiveTexts;
};

}  // namespace gsm

#endif /* !HOW_TO_PLAY_HPP_ */
