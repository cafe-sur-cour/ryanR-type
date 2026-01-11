/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#ifndef RESULTSSTATE_HPP_
#define RESULTSSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "ui/manager/UIManager.hpp"
#include "ui/elements/Text.hpp"
#include "ui/elements/focusable/Button.hpp"
#include "ui/core/UILayout.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../colors.hpp"

namespace gsm {

class ResultsState : public AGameState {
    public:
        ResultsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager, bool isWin);
        ~ResultsState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;

    private:
        void updateUserStats();
        void renderUI();

        bool _isWin;
        std::unique_ptr<ui::UIManager> _uiManager;
        std::unique_ptr<MouseInputHandler> _mouseHandler;
        std::shared_ptr<ui::Text> _resultText;
        std::shared_ptr<ui::Button> _leaveButton;
        std::shared_ptr<ui::UILayout> _bottomRightLayout;
};

}  // namespace gsm

#endif  // RESULTSSTATE_HPP_
