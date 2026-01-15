/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef CHAT_STATE_HPP_
#define CHAT_STATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/focusable/TextInput.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ClientNetwork.hpp"

namespace gsm {

class ChatState : public AGameState {
public:
    ChatState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~ChatState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Chat"; }

private:
    void renderUI();
    void onBackButtonClicked();
    void onSendMessage(const std::string& text);

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::UILayout> _mainLayout;

    std::shared_ptr<ui::Button> _backButton;
    std::shared_ptr<ui::Text> _titleText;
    std::shared_ptr<ui::TextInput> _messageInput;
    std::shared_ptr<ui::Button> _sendButton;
    std::shared_ptr<ui::UILayout> _messagesContainer;
};

}  // namespace gsm

#endif /* !CHAT_STATE_HPP_ */
