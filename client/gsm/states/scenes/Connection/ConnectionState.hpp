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
#include "../../../../ui/elements/SpritePreview.hpp"


namespace gsm {

class ConnectionState : public AGameState {
    public:
        ConnectionState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~ConnectionState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "Connecting"; }

    private:
        void renderUI();
        void updateUIStatus();

        std::unique_ptr<ui::UIManager> _uiManager;
        std::unique_ptr<MouseInputHandler> _mouseHandler;
        std::shared_ptr<ui::Background> _background;
        std::shared_ptr<ui::TextInput> _ipInput;
        std::shared_ptr<ui::TextInput> _portInput;
        std::shared_ptr<ui::Button> _connectButton;
        std::shared_ptr<ui::Button> _levelEditorButton;
        std::shared_ptr<ui::Button> _quitButton;
        std::shared_ptr<ui::Text> _spacer;
        std::shared_ptr<ui::UILayout> _layout;
        std::shared_ptr<ui::SpritePreview> _loadingAnimation;
        std::shared_ptr<ui::UILayout> _loadingLayout;
        bool _wasConnected = false;
};

} // namespace gsm

#endif /* !CONNECTION_STATE_HPP_ */
