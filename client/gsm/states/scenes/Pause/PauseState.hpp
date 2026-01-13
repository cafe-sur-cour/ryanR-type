/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PauseState
*/

#ifndef PAUSESTATE_HPP_
#define PAUSESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"

namespace gsm {

class PauseState : public AGameState {
public:
    PauseState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~PauseState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Paused"; }

private:
    void renderUI();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::UILayout> _menuLayout;
    std::shared_ptr<ui::Button> _resumeButton;
    std::shared_ptr<ui::Button> _settingsButton;
    std::shared_ptr<ui::Button> _leaveButton;
};

}  // namespace gsm

#endif  // PAUSESTATE_HPP_
