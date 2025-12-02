/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#ifndef MAINMENUSTATE_HPP_
#define MAINMENUSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../../MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/manager/UIManager.hpp"


namespace gsm {

class MainMenuState : public AGameState {
public:
    MainMenuState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ecs::ResourceManager> resourceManager);
    ~MainMenuState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::shared_ptr<ui::Button> _playButton;
    std::shared_ptr<ui::Button> _quitButton;
    std::unique_ptr<ui::UIManager> _uiManager;
};

}  // namespace gsm

#endif  // MAINMENUSTATE_HPP_
