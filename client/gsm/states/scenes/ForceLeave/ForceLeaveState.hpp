/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ForceLeaveState
*/

#ifndef FORCELEAVESTATE_HPP_
#define FORCELEAVESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "ui/manager/UIManager.hpp"
#include "ui/elements/Text.hpp"
#include "ui/elements/focusable/Button.hpp"
#include "ui/core/UILayout.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../colors.hpp"
#include "../../../../../common/constants.hpp"

namespace gsm {

class ForceLeaveState : public AGameState {
    public:
        ForceLeaveState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager, constants::ForceLeaveType leaveType);
        ~ForceLeaveState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "ForceLeave"; }

    private:
        void renderUI();

        constants::ForceLeaveType _leaveType;
        std::unique_ptr<ui::UIManager> _uiManager;
        std::unique_ptr<MouseInputHandler> _mouseHandler;
        std::shared_ptr<ui::Text> _reasonText;
        std::shared_ptr<ui::Button> _leaveButton;
        std::shared_ptr<ui::UILayout> _bottomRightLayout;
};

}  // namespace gsm

#endif  // FORCELEAVESTATE_HPP_