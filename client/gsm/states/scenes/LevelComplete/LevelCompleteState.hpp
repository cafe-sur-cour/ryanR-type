/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelCompleteState
*/

#ifndef CLIENT_LEVELCOMPLETESTATE_HPP_
#define CLIENT_LEVELCOMPLETESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "ui/manager/UIManager.hpp"
#include "ui/elements/Text.hpp"
#include "../../../../colors.hpp"

namespace gsm {

class LevelCompleteState : public AGameState {
    public:
        LevelCompleteState(std::shared_ptr<IGameStateMachine> gsm,
            std::shared_ptr<ResourceManager> resourceManager);
        ~LevelCompleteState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "Level Complete"; }

        void onNextLevel();

    private:
        std::unique_ptr<ui::UIManager> _uiManager;
        std::shared_ptr<ui::Text> _titleText;
        std::shared_ptr<ui::Text> _subtitleText;
        bool _waitingForNextLevel;
};

}  // namespace gsm

#endif  // CLIENT_LEVELCOMPLETESTATE_HPP_
