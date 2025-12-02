/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MainMenuState
*/

#ifndef MAINMENUSTATE_HPP_
#define MAINMENUSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/resourceManager/ResourceManager.hpp"
#include "../../../../MouseInputHandler.hpp"
#include "../../../../ui/Button.hpp"

namespace gsm {

class MainMenuState : public AGameState {
public:
    MainMenuState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~MainMenuState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();

private:
    std::shared_ptr<ResourceManager> _resourceManager;
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::shared_ptr<ui::Button> _testButton;
};

}  // namespace gsm

#endif  // MAINMENUSTATE_HPP_
