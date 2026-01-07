/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef REGISTER_STATE_HPP_
#define REGISTER_STATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/focusable/TextInput.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"

namespace gsm {

class RegisterState : public AGameState {
public:
    RegisterState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~RegisterState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;
    
    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::UILayout> _mainLayout;
    
    std::shared_ptr<ui::TextInput> _usernameInput;
    std::shared_ptr<ui::TextInput> _passwordInput;
    std::shared_ptr<ui::TextInput> _confirmPasswordInput;
    
    std::shared_ptr<ui::Button> _registerButton;
    std::shared_ptr<ui::Button> _backButton;
};

}  // namespace gsm

#endif /* !REGISTER_STATE_HPP_ */
