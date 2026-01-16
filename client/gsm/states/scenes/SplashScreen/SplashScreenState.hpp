/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** SplashScreenState Header
*/

#ifndef SPLASHSCREEN_STATE_HPP_
#define SPLASHSCREEN_STATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/elements/Text.hpp"
#include <memory>

namespace gsm {

class SplashScreenState : public AGameState {
    public:
        SplashScreenState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~SplashScreenState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "SplashScreen"; }

    private:
        void renderUI();

        std::unique_ptr<ui::UIManager> _uiManager;
        std::shared_ptr<ui::Text> _titleText;
        std::shared_ptr<ui::Text> _subtitleText;
        float _animationTime = 0.0f;
        float _currentFontSize = constants::SPLASHSCREEN_START_FONT_SIZE;
        const float _startFontSize = constants::SPLASHSCREEN_START_FONT_SIZE;
        const float _targetFontSize = constants::SPLASHSCREEN_TARGET_FONT_SIZE;
        const float _growDuration = constants::SPLASHSCREEN_GROW_DURATION;
        const float _totalDuration = constants::SPLASHSCREEN_TOTAL_DURATION;
};

}  // namespace gsm

#endif /* !SPLASHSCREEN_STATE_HPP_ */
