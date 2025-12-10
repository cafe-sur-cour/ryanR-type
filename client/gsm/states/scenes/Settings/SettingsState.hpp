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
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../../common/types/Vector2f.hpp"
#include "../../../../../common/InputMapping/InputAction.hpp"
#include "../../../../../libs/Multimedia/EventTypes.hpp"
#include <optional>

namespace gsm {

class SettingsState : public AGameState {
public:
    SettingsState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~SettingsState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();
    void cycleColorBlindnessFilter();
    void toggleHighContrastFilter();
    void updateBrightnessFilter(float value);
    void cycleUIScale();
    void updateMusicVolume(float value);
    void updateSoundVolume(float value);
    void updateToggleValue(bool value);

    void startKeyRebind(ecs::InputAction action, float direction, std::shared_ptr<ui::Button> button);
    void handleKeyRebind(gfx::EventType newKey);
    void updateKeyBindingButtonText(
        std::shared_ptr<ui::Button> button,
        ecs::InputAction action, float direction,
        const std::string& label
    );
    std::string getActionName(ecs::InputAction action) const;

private:
    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::shared_ptr<ui::Button> _backButton;
    std::shared_ptr<ui::Button> _highContrastButton;
    std::shared_ptr<ui::Button> _colorBlindnessButton;
    std::shared_ptr<ui::Slider> _brightnessSlider;
    std::shared_ptr<ui::Slider> _musicVolumeSlider;
    std::shared_ptr<ui::Slider> _soundVolumeSlider;
    std::shared_ptr<ui::ToggleSwitch> _toggleSwitch;
    std::shared_ptr<ui::Button> _scaleButton;
    std::unique_ptr<ui::UIManager> _uiManager;
    std::shared_ptr<ui::UILayout> _settingsLayout;
    std::shared_ptr<ui::UILayout> _leftColumnLayout;
    std::shared_ptr<ui::UILayout> _rightColumnLayout;
    std::shared_ptr<ui::UILayout> _titleLayout;
    math::Vector2f _savedViewCenter;

    std::shared_ptr<ui::Button> _moveUpKeyButton;
    std::shared_ptr<ui::Button> _moveDownKeyButton;
    std::shared_ptr<ui::Button> _moveLeftKeyButton;
    std::shared_ptr<ui::Button> _moveRightKeyButton;
    std::shared_ptr<ui::Button> _shootKeyButton;

    bool _isWaitingForKey = false;
    std::optional<ecs::InputAction> _actionToRebind;
    float _rebindDirection = 1.0f;
    std::string _rebindLabel;
    std::shared_ptr<ui::Button> _buttonToUpdate;
    gfx::EventType _originalKey = gfx::EventType::NOTHING;

    std::string getColorBlindnessText(int state);
    std::string getUIScaleText(ui::UIScale scale);
};

}  // namespace gsm

#endif  // SETTINGSSTATE_HPP_
