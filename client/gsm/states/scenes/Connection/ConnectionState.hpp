/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Header
*/

#ifndef CONNECTION_STATE_HPP_
#define CONNECTION_STATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/focusable/TextInput.hpp"
#include "../../../../ui/elements/Text.hpp"
#include <memory>

namespace gsm {

class ConnectionState : public AGameState {
    public:
        ConnectionState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~ConnectionState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;

    private:
        void renderUI();
        void updateUIStatus();

        std::unique_ptr<ui::UIManager> _uiManager;
        std::unique_ptr<MouseInputHandler> _mouseHandler;
        std::shared_ptr<ui::Background> _background;
        std::shared_ptr<ui::TextInput> _ipInput;
        std::shared_ptr<ui::TextInput> _portInput;
        std::shared_ptr<ui::Button> _connectButton;
        std::shared_ptr<ui::Button> _backButton;
        std::shared_ptr<ui::UILayout> _layout;
        bool _wasConnected = false;
};

} // namespace gsm

#endif /* !CONNECTION_STATE_HPP_ */
