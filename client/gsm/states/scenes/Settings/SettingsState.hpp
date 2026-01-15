/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsState
*/

#ifndef SETTINGSSTATE_HPP_
#define SETTINGSSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/focusable/Slider.hpp"
#include "../../../../ui/elements/focusable/ToggleSwitch.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../../common/types/Vector2f.hpp"
#include "../../../../../common/InputMapping/InputAction.hpp"
#include "../../../../../libs/Multimedia/EventTypes.hpp"
#include <optional>
#include "../../../../SettingsManager.hpp"

namespace gsm {

class SettingsState : public AGameState {
public:
    SettingsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~SettingsState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;
    std::string getStateName() const override { return "Settings"; }

private:
    void renderUI();
    void cycleColorBlindnessFilter();
    void toggleHighContrastFilter();
    void updateBrightnessFilter(float value);
    void applyColorBlindnessFilter(int state);
    void applyHighContrastFilter(bool enabled);
    void cycleUIScale();
    void updateMusicVolume(float value);
    void updateSoundVolume(float value);
    void updateToggleValue(bool value);
    void cycleScreenResolution();
    void updateTargetFPS(int fps);
    void updateRenderQuality(float quality);
    void updateInGameMetrics(bool enabled);
    void setScreenResolution(SettingsConfig::ScreenResolution resolution);
    void updateResolutionButtonColors(SettingsConfig::ScreenResolution current);

    void startKeyRebind(ecs::RemappableAction action, bool rebindPrimary, std::shared_ptr<ui::Button> button);
    void handleKeyRebind(gfx::EventType newKey);
    void updateKeyBindingButtonText(std::shared_ptr<ui::Button> button, ecs::RemappableAction action, bool isPrimary);
    std::string getRemappableActionName(ecs::RemappableAction action) const;

    std::string getScreenResolutionText(SettingsConfig::ScreenResolution resolution);

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::shared_ptr<ui::Button> _backButton;
    std::shared_ptr<ui::Button> _highContrastButton;
    std::shared_ptr<ui::Button> _colorBlindnessButton;
    std::shared_ptr<ui::Slider> _brightnessSlider;
    std::shared_ptr<ui::Slider> _musicVolumeSlider;
    std::shared_ptr<ui::Slider> _soundVolumeSlider;
    std::shared_ptr<ui::ToggleSwitch> _toggleSwitch;
    std::shared_ptr<ui::Text> _toggleLabel;
    std::shared_ptr<ui::UILayout> _toggleLayout;
    std::vector<std::shared_ptr<ui::Button>> _resolutionButtons;
    std::shared_ptr<ui::Slider> _fpsSlider;
    std::shared_ptr<ui::Slider> _renderQualitySlider;
    std::shared_ptr<ui::Button> _scaleButton;
    std::unique_ptr<ui::UIManager> _uiManager;
    std::shared_ptr<ui::UILayout> _settingsLayout;
    std::shared_ptr<ui::UILayout> _leftColumnLayout;
    std::shared_ptr<ui::UILayout> _rightColumnLayout;
    std::shared_ptr<ui::UILayout> _centerColumnLayout;
    std::shared_ptr<ui::UILayout> _titleLayout;
    std::shared_ptr<ui::Background> _background;
    math::Vector2f _savedViewCenter;

    std::shared_ptr<SettingsManager> _settingsManager;

    std::shared_ptr<ui::UILayout> _moveUpLayout;
    std::shared_ptr<ui::Text> _moveUpLabel;
    std::shared_ptr<ui::Button> _moveUpPrimaryButton;
    std::shared_ptr<ui::Button> _moveUpSecondaryButton;

    std::shared_ptr<ui::UILayout> _moveDownLayout;
    std::shared_ptr<ui::Text> _moveDownLabel;
    std::shared_ptr<ui::Button> _moveDownPrimaryButton;
    std::shared_ptr<ui::Button> _moveDownSecondaryButton;

    std::shared_ptr<ui::UILayout> _moveLeftLayout;
    std::shared_ptr<ui::Text> _moveLeftLabel;
    std::shared_ptr<ui::Button> _moveLeftPrimaryButton;
    std::shared_ptr<ui::Button> _moveLeftSecondaryButton;

    std::shared_ptr<ui::UILayout> _moveRightLayout;
    std::shared_ptr<ui::Text> _moveRightLabel;
    std::shared_ptr<ui::Button> _moveRightPrimaryButton;
    std::shared_ptr<ui::Button> _moveRightSecondaryButton;

    std::shared_ptr<ui::UILayout> _shootLayout;
    std::shared_ptr<ui::Text> _shootLabel;
    std::shared_ptr<ui::Button> _shootPrimaryButton;
    std::shared_ptr<ui::Button> _shootSecondaryButton;

    std::shared_ptr<ui::UILayout> _forceLayout;
    std::shared_ptr<ui::Text> _forceLabel;
    std::shared_ptr<ui::Button> _forcePrimaryButton;
    std::shared_ptr<ui::Button> _forceSecondaryButton;

    bool _isWaitingForKey = false;
    std::optional<ecs::RemappableAction> _actionToRebind;
    bool _rebindingPrimary = true;
    std::string _rebindLabel;
    std::shared_ptr<ui::Button> _buttonToUpdate;
    gfx::EventType _originalKey = gfx::EventType::NOTHING;

    std::shared_ptr<ui::ToggleSwitch> _inGameMetricsToggle;
    std::shared_ptr<ui::Text> _inGameMetricsLabel;

    std::string getColorBlindnessText(int state);
    std::string getUIScaleText(ui::UIScale scale);
};

}  // namespace gsm

#endif  // SETTINGSSTATE_HPP_
