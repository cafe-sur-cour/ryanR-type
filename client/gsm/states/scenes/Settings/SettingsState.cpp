/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../../common/InputMapping/InputMappingManager.hpp"
#include "../../../../initResourcesManager/GraphicalInputProvider.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"

namespace gsm {

SettingsState::SettingsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager
) : AGameState(gsm, resourceManager) {
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    _background = std::make_shared<ui::Background>(_resourceManager);

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    auto inputProvider = _resourceManager->get<ecs::IInputProvider>();

    ui::LayoutConfig settingsConfig;
    settingsConfig.direction = ui::LayoutDirection::Vertical;
    settingsConfig.alignment = ui::LayoutAlignment::Center;
    settingsConfig.spacing = 20.0f;
    settingsConfig.padding = math::Vector2f(0.0f, 0.0f);
    settingsConfig.anchorX = ui::AnchorX::Center;
    settingsConfig.anchorY = ui::AnchorY::Center;
    settingsConfig.offset = math::Vector2f(0.0f, 0.0f);

    _settingsLayout = std::make_shared<ui::UILayout>(resourceManager, settingsConfig);
    _settingsLayout->setSize(math::Vector2f(1300.f, 650.f));

    ui::LayoutConfig columnsConfig;
    columnsConfig.direction = ui::LayoutDirection::Horizontal;
    columnsConfig.alignment = ui::LayoutAlignment::Center;
    columnsConfig.spacing = 100.0f;
    columnsConfig.padding = math::Vector2f(0.0f, 0.0f);

    auto columnsLayout = std::make_shared<ui::UILayout>(resourceManager, columnsConfig);
    columnsLayout->setSize(math::Vector2f(1300.f, 600.f));

    ui::LayoutConfig columnConfig;
    columnConfig.direction = ui::LayoutDirection::Vertical;
    columnConfig.alignment = ui::LayoutAlignment::Center;
    columnConfig.spacing = 15.0f;
    columnConfig.padding = math::Vector2f(0.0f, 0.0f);

    _leftColumnLayout = std::make_shared<ui::UILayout>(resourceManager, columnConfig);
    _leftColumnLayout->setSize(math::Vector2f(400.f, 600.f));

    _rightColumnLayout = std::make_shared<ui::UILayout>(resourceManager, columnConfig);
    _rightColumnLayout->setSize(math::Vector2f(400.f, 600.f));

    _centerColumnLayout = std::make_shared<ui::UILayout>(resourceManager, columnConfig);
    _centerColumnLayout->setSize(math::Vector2f(250.f, 600.f));

    _scaleButton = std::make_shared<ui::Button>(resourceManager);
    _scaleButton->setText(getUIScaleText(config->getUIScale()));
    _scaleButton->setSize(math::Vector2f(380.f, 55.f));
    _scaleButton->setOnRelease([this]() {
        cycleUIScale();
    });
    _scaleButton->setOnActivated([this]() {
        cycleUIScale();
    });

    _brightnessSlider = std::make_shared<ui::Slider>(resourceManager);
    _brightnessSlider->setLabel("Brightness");
    _brightnessSlider->setMinValue(25.0f);
    _brightnessSlider->setMaxValue(150.0f);
    _brightnessSlider->setValue(config->getBrightnessValue() * 100.0f);
    _brightnessSlider->setStep(5.0f);
    _brightnessSlider->setSize(math::Vector2f(380.f, 55.f));
    _brightnessSlider->setOnValueChanged([this](float value) {
        updateBrightnessFilter(value);
    });

    _musicVolumeSlider = std::make_shared<ui::Slider>(resourceManager);
    _musicVolumeSlider->setLabel("Music Volume");
    _musicVolumeSlider->setMinValue(0.0f);
    _musicVolumeSlider->setMaxValue(100.0f);
    _musicVolumeSlider->setValue(config->getMusicVolume());
    _musicVolumeSlider->setStep(5.0f);
    _musicVolumeSlider->setSize(math::Vector2f(380.f, 55.f));
    _musicVolumeSlider->setOnValueChanged([this](float value) {
        updateMusicVolume(value);
    });

    _soundVolumeSlider = std::make_shared<ui::Slider>(resourceManager);
    _soundVolumeSlider->setLabel("Sound Volume");
    _soundVolumeSlider->setMinValue(0.0f);
    _soundVolumeSlider->setMaxValue(100.0f);
    _soundVolumeSlider->setValue(config->getSoundVolume());
    _soundVolumeSlider->setStep(5.0f);
    _soundVolumeSlider->setSize(math::Vector2f(380.f, 55.f));
    _soundVolumeSlider->setOnValueChanged([this](float value) {
        updateSoundVolume(value);
    });

    _toggleSwitch = std::make_shared<ui::ToggleSwitch>(resourceManager);
    _toggleSwitch->setOnText("TOGGLE");
    _toggleSwitch->setOffText("HOLD");
    auto graphicalProvider =
        std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(inputProvider);
    if (graphicalProvider) {
        _toggleSwitch->setValue(graphicalProvider->isToggleMode());
    } else {
        _toggleSwitch->setValue(false);
    }
    _toggleSwitch->setSize(math::Vector2f(380.f, 55.f));
    _toggleSwitch->setOnValueChanged([this](bool value) {
        updateToggleValue(value);
    });

    _toggleLabel = std::make_shared<ui::Text>(resourceManager);
    _toggleLabel->setText("Control Mode");
    _toggleLabel->setFontSize(20);
    _toggleLabel->setSize(math::Vector2f(380.f, 30.f));

    std::vector<SettingsConfig::ScreenResolution> resolutions = {
        SettingsConfig::ScreenResolution::FULLSCREEN,
        SettingsConfig::ScreenResolution::RES_1920x1080,
        SettingsConfig::ScreenResolution::RES_1280x720,
        SettingsConfig::ScreenResolution::RES_1024x768,
        SettingsConfig::ScreenResolution::RES_800x600
    };

    for (auto res : resolutions) {
        auto button = std::make_shared<ui::Button>(resourceManager);
        button->setText(getScreenResolutionText(res));
        button->setSize(math::Vector2f(250.f, 55.f));
        button->setOnRelease([this, res]() {
            setScreenResolution(res);
        });
        button->setOnActivated([this, res]() {
            setScreenResolution(res);
        });
        _resolutionButtons.push_back(button);
    }

    updateResolutionButtonColors(config->getScreenResolution());

    _fpsSlider = std::make_shared<ui::Slider>(resourceManager);
    _fpsSlider->setLabel("FPS Limit");
    _fpsSlider->setMinValue(15.0f);
    _fpsSlider->setMaxValue(160.0f);
    _fpsSlider->setValue(static_cast<float>(config->getTargetFPS()));
    _fpsSlider->setStep(5.0f);
    _fpsSlider->setSize(math::Vector2f(250.f, 55.f));
    _fpsSlider->setShowPercentage(false);
    _fpsSlider->setOnValueChanged([this](float value) {
        updateTargetFPS(static_cast<int>(std::round(value)));
    });

    _renderQualitySlider = std::make_shared<ui::Slider>(resourceManager);
    _renderQualitySlider->setLabel("Render Quality");
    _renderQualitySlider->setMinValue(30.0f);
    _renderQualitySlider->setMaxValue(100.0f);
    _renderQualitySlider->setValue(config->getRenderQuality() * 100.0f);
    _renderQualitySlider->setStep(5.0f);
    _renderQualitySlider->setSize(math::Vector2f(250.f, 55.f));
    _renderQualitySlider->setShowPercentage(true);
    _renderQualitySlider->setOnValueChanged([this](float value) {
        updateRenderQuality(value / 100.0f);
    });

    _inGameMetricsToggle = std::make_shared<ui::ToggleSwitch>(resourceManager);
    _inGameMetricsToggle->setOnText("ON");
    _inGameMetricsToggle->setOffText("OFF");
    _inGameMetricsToggle->setValue(config->isInGameMetricsEnabled());
    _inGameMetricsToggle->setSize(math::Vector2f(250.f, 55.f));
    _inGameMetricsToggle->setOnValueChanged([this](bool value) {
        updateInGameMetrics(value);
    });

    _inGameMetricsLabel = std::make_shared<ui::Text>(resourceManager);
    _inGameMetricsLabel->setText("In-Game Metrics");
    _inGameMetricsLabel->setFontSize(16);
    _inGameMetricsLabel->setSize(math::Vector2f(250.f, 25.f));

    _colorBlindnessButton = std::make_shared<ui::Button>(resourceManager);
    _colorBlindnessButton->setText(getColorBlindnessText(config->getColorBlindnessState()));
    _colorBlindnessButton->setSize(math::Vector2f(380.f, 55.f));
    _colorBlindnessButton->setOnRelease([this]() {
        cycleColorBlindnessFilter();
    });
    _colorBlindnessButton->setOnActivated([this]() {
        cycleColorBlindnessFilter();
    });

    _highContrastButton = std::make_shared<ui::Button>(resourceManager);
    _highContrastButton->setText(
        config->isHighContrastEnabled() ? "High Contrast: ON" : "High Contrast: OFF"
    );
    _highContrastButton->setSize(math::Vector2f(380.f, 55.f));
    _highContrastButton->setOnRelease([this]() {
        toggleHighContrastFilter();
    });
    _highContrastButton->setOnActivated([this]() {
        toggleHighContrastFilter();
    });

    _backButton = std::make_shared<ui::Button>(resourceManager);
    _backButton->setText("Back");
    _backButton->setSize(math::Vector2f(380.f, 55.f));
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnRelease([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });
    _backButton->setOnActivated([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    ui::LayoutConfig verticalConfig;
    verticalConfig.direction = ui::LayoutDirection::Vertical;
    verticalConfig.alignment = ui::LayoutAlignment::Center;
    verticalConfig.spacing = 1.0f;
    verticalConfig.padding = math::Vector2f(0.0f, 0.0f);

    ui::LayoutConfig mappingVerticalConfig;
    mappingVerticalConfig.direction = ui::LayoutDirection::Vertical;
    mappingVerticalConfig.alignment = ui::LayoutAlignment::Center;
    mappingVerticalConfig.spacing = 3.0f;
    mappingVerticalConfig.padding = math::Vector2f(0.0f, 0.0f);

    ui::LayoutConfig horizontalConfig;
    horizontalConfig.direction = ui::LayoutDirection::Horizontal;
    horizontalConfig.alignment = ui::LayoutAlignment::Center;
    horizontalConfig.spacing = 20.0f;
    horizontalConfig.padding = math::Vector2f(0.0f, 0.0f);

    _moveUpLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _moveUpLayout->setSize(math::Vector2f(380.f, 85.f));

    _moveUpLabel = std::make_shared<ui::Text>(resourceManager);
    _moveUpLabel->setText("Move Up");
    _moveUpLabel->setFontSize(20);
    _moveUpLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveUpButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveUpButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveUpPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveUpPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveUpPrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _moveUpPrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _moveUpPrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _moveUpPrimaryButton->setText("1");
    updateKeyBindingButtonText(_moveUpPrimaryButton, ecs::RemappableAction::MOVE_UP, true);
    _moveUpPrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_UP, true, _moveUpPrimaryButton);
    });
    _moveUpPrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_UP, true, _moveUpPrimaryButton);
    });

    _moveUpSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveUpSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveUpSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _moveUpSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _moveUpSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _moveUpSecondaryButton->setText("2");
    updateKeyBindingButtonText(_moveUpSecondaryButton, ecs::RemappableAction::MOVE_UP, false);
    _moveUpSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_UP, false, _moveUpSecondaryButton);
    });
    _moveUpSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_UP, false, _moveUpSecondaryButton);
    });

    moveUpButtonsLayout->addElement(_moveUpPrimaryButton);
    moveUpButtonsLayout->addElement(_moveUpSecondaryButton);

    _moveUpLayout->addElement(_moveUpLabel);
    _moveUpLayout->addElement(moveUpButtonsLayout);

    _moveDownLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _moveDownLayout->setSize(math::Vector2f(380.f, 85.f));

    _moveDownLabel = std::make_shared<ui::Text>(resourceManager);
    _moveDownLabel->setText("Move Down");
    _moveDownLabel->setFontSize(20);
    _moveDownLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveDownButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveDownButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveDownPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveDownPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveDownPrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _moveDownPrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _moveDownPrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _moveDownPrimaryButton->setText("1");
    updateKeyBindingButtonText(_moveDownPrimaryButton, ecs::RemappableAction::MOVE_DOWN, true);
    _moveDownPrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_DOWN, true, _moveDownPrimaryButton);
    });
    _moveDownPrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_DOWN, true, _moveDownPrimaryButton);
    });

    _moveDownSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveDownSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveDownSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _moveDownSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _moveDownSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _moveDownSecondaryButton->setText("2");
    updateKeyBindingButtonText(_moveDownSecondaryButton,
        ecs::RemappableAction::MOVE_DOWN, false);
    _moveDownSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_DOWN, false, _moveDownSecondaryButton);
    });
    _moveDownSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_DOWN, false, _moveDownSecondaryButton);
    });

    moveDownButtonsLayout->addElement(_moveDownPrimaryButton);
    moveDownButtonsLayout->addElement(_moveDownSecondaryButton);

    _moveDownLayout->addElement(_moveDownLabel);
    _moveDownLayout->addElement(moveDownButtonsLayout);

    _moveLeftLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _moveLeftLayout->setSize(math::Vector2f(380.f, 85.f));

    _moveLeftLabel = std::make_shared<ui::Text>(resourceManager);
    _moveLeftLabel->setText("Move Left");
    _moveLeftLabel->setFontSize(20);
    _moveLeftLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveLeftButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveLeftButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveLeftPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveLeftPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveLeftPrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _moveLeftPrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _moveLeftPrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _moveLeftPrimaryButton->setText("1");
    updateKeyBindingButtonText(_moveLeftPrimaryButton, ecs::RemappableAction::MOVE_LEFT, true);
    _moveLeftPrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_LEFT, true, _moveLeftPrimaryButton);
    });
    _moveLeftPrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_LEFT, true, _moveLeftPrimaryButton);
    });

    _moveLeftSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveLeftSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveLeftSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _moveLeftSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _moveLeftSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _moveLeftSecondaryButton->setText("2");
    updateKeyBindingButtonText(_moveLeftSecondaryButton,
        ecs::RemappableAction::MOVE_LEFT, false);
    _moveLeftSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_LEFT, false, _moveLeftSecondaryButton);
    });
    _moveLeftSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_LEFT, false, _moveLeftSecondaryButton);
    });

    moveLeftButtonsLayout->addElement(_moveLeftPrimaryButton);
    moveLeftButtonsLayout->addElement(_moveLeftSecondaryButton);

    _moveLeftLayout->addElement(_moveLeftLabel);
    _moveLeftLayout->addElement(moveLeftButtonsLayout);

    _moveRightLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _moveRightLayout->setSize(math::Vector2f(380.f, 85.f));

    _moveRightLabel = std::make_shared<ui::Text>(resourceManager);
    _moveRightLabel->setText("Move Right");
    _moveRightLabel->setFontSize(20);
    _moveRightLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveRightButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveRightButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveRightPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveRightPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveRightPrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _moveRightPrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _moveRightPrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _moveRightPrimaryButton->setText("1");
    updateKeyBindingButtonText(_moveRightPrimaryButton,
        ecs::RemappableAction::MOVE_RIGHT, true);
    _moveRightPrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_RIGHT, true, _moveRightPrimaryButton);
    });
    _moveRightPrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_RIGHT, true, _moveRightPrimaryButton);
    });

    _moveRightSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveRightSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveRightSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _moveRightSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _moveRightSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _moveRightSecondaryButton->setText("2");
    updateKeyBindingButtonText(_moveRightSecondaryButton,
        ecs::RemappableAction::MOVE_RIGHT, false);
    _moveRightSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_RIGHT, false, _moveRightSecondaryButton);
    });
    _moveRightSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::MOVE_RIGHT, false, _moveRightSecondaryButton);
    });

    moveRightButtonsLayout->addElement(_moveRightPrimaryButton);
    moveRightButtonsLayout->addElement(_moveRightSecondaryButton);

    _moveRightLayout->addElement(_moveRightLabel);
    _moveRightLayout->addElement(moveRightButtonsLayout);

    _shootLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _shootLayout->setSize(math::Vector2f(380.f, 85.f));

    _shootLabel = std::make_shared<ui::Text>(resourceManager);
    _shootLabel->setText("Shoot");
    _shootLabel->setFontSize(20);
    _shootLabel->setSize(math::Vector2f(380.f, 30.f));

    auto shootButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    shootButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _shootPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _shootPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _shootPrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _shootPrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _shootPrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _shootPrimaryButton->setText("1");
    updateKeyBindingButtonText(_shootPrimaryButton, ecs::RemappableAction::SHOOT, true);
    _shootPrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::SHOOT, true, _shootPrimaryButton);
    });
    _shootPrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::SHOOT, true, _shootPrimaryButton);
    });

    _shootSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _shootSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _shootSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _shootSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _shootSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _shootSecondaryButton->setText("2");
    updateKeyBindingButtonText(_shootSecondaryButton, ecs::RemappableAction::SHOOT, false);
    _shootSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::SHOOT, false, _shootSecondaryButton);
    });
    _shootSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::SHOOT, false, _shootSecondaryButton);
    });

    shootButtonsLayout->addElement(_shootPrimaryButton);
    shootButtonsLayout->addElement(_shootSecondaryButton);

    _shootLayout->addElement(_shootLabel);
    _shootLayout->addElement(shootButtonsLayout);

    _forceLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _forceLayout->setSize(math::Vector2f(380.f, 85.f));

    _forceLabel = std::make_shared<ui::Text>(resourceManager);
    _forceLabel->setText("Force");
    _forceLabel->setFontSize(20);
    _forceLabel->setSize(math::Vector2f(380.f, 30.f));

    auto forceButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    forceButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _forcePrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _forcePrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _forcePrimaryButton->setNormalColor(colors::BUTTON_PRIMARY);
    _forcePrimaryButton->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
    _forcePrimaryButton->setFocusedColor(colors::BUTTON_PRIMARY_PRESSED);
    _forcePrimaryButton->setText("1");
    updateKeyBindingButtonText(_forcePrimaryButton, ecs::RemappableAction::FORCE, true);
    _forcePrimaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::FORCE, true, _forcePrimaryButton);
    });
    _forcePrimaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::FORCE, true, _forcePrimaryButton);
    });

    _forceSecondaryButton = std::make_shared<ui::Button>(resourceManager);
    _forceSecondaryButton->setSize(math::Vector2f(180.f, 50.f));
    _forceSecondaryButton->setNormalColor(colors::BUTTON_SECONDARY);
    _forceSecondaryButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _forceSecondaryButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _forceSecondaryButton->setText("2");
    updateKeyBindingButtonText(_forceSecondaryButton, ecs::RemappableAction::FORCE, false);
    _forceSecondaryButton->setOnRelease([this]() {
        startKeyRebind(ecs::RemappableAction::FORCE, false, _forceSecondaryButton);
    });
    _forceSecondaryButton->setOnActivated([this]() {
        startKeyRebind(ecs::RemappableAction::FORCE, false, _forceSecondaryButton);
    });

    forceButtonsLayout->addElement(_forcePrimaryButton);
    forceButtonsLayout->addElement(_forceSecondaryButton);

    _forceLayout->addElement(_forceLabel);
    _forceLayout->addElement(forceButtonsLayout);

    _toggleLayout = std::make_shared<ui::UILayout>(resourceManager, mappingVerticalConfig);
    _toggleLayout->setSize(math::Vector2f(380.f, 85.f));
    _toggleLayout->addElement(_toggleLabel);
    _toggleLayout->addElement(_toggleSwitch);

    _leftColumnLayout->addElement(_musicVolumeSlider);
    _leftColumnLayout->addElement(_soundVolumeSlider);
    _leftColumnLayout->addElement(_brightnessSlider);
    _leftColumnLayout->addElement(_scaleButton);
    _leftColumnLayout->addElement(_colorBlindnessButton);
    _leftColumnLayout->addElement(_highContrastButton);
    _leftColumnLayout->addElement(_backButton);

    _rightColumnLayout->addElement(_moveUpLayout);
    _rightColumnLayout->addElement(_moveDownLayout);
    _rightColumnLayout->addElement(_moveLeftLayout);
    _rightColumnLayout->addElement(_moveRightLayout);
    _rightColumnLayout->addElement(_shootLayout);
    _rightColumnLayout->addElement(_forceLayout);
    _rightColumnLayout->addElement(_toggleLayout);

    _centerColumnLayout->addElement(_fpsSlider);
    _centerColumnLayout->addElement(_renderQualitySlider);
    _centerColumnLayout->addElement(_inGameMetricsLabel);
    _centerColumnLayout->addElement(_inGameMetricsToggle);

    for (auto& button : _resolutionButtons) {
        _centerColumnLayout->addElement(button);
    }

    columnsLayout->addElement(_leftColumnLayout);
    columnsLayout->addElement(_centerColumnLayout);
    columnsLayout->addElement(_rightColumnLayout);

    _settingsLayout->addElement(_titleLayout);
    _settingsLayout->addElement(columnsLayout);
    _uiManager->addElement(_settingsLayout);

    _settingsManager = _resourceManager->get<SettingsManager>();
}

void SettingsState::enter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    _savedViewCenter = window->getViewCenter();
    auto logicalSize = window->getLogicalSize();
    window->setViewCenter(
        static_cast<float>(logicalSize.first) / 2.0f,
        static_cast<float>(logicalSize.second) / 2.0f
    );

    _uiManager->setOnBack([this]() {
        if (!_isWaitingForKey) {
            if (auto stateMachine = this->_gsm.lock()) {
                stateMachine->requestStatePop();
            }
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_CHAT, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
}

void SettingsState::update(float deltaTime) {
    (void)deltaTime;

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();

    if (_isWaitingForKey) {
        if (eventResult == gfx::EventType::ESCAPE) {
            _isWaitingForKey = false;
            if (_buttonToUpdate && _actionToRebind.has_value()) {
                updateKeyBindingButtonText(
                    _buttonToUpdate,
                    _actionToRebind.value(),
                    _rebindingPrimary
                );
            }
            _actionToRebind.reset();
            _buttonToUpdate.reset();
            _rebindLabel.clear();
        } else if (eventResult != gfx::EventType::NOTHING &&
            mappingManager && mappingManager->isKeyboardKey(eventResult)) {
            handleKeyRebind(eventResult);
        }
        _background->update(deltaTime);
        _uiManager->update(deltaTime);
        renderUI();
        return;
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _uiManager->handleMouseInput(mousePos, mousePressed);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _background->update(deltaTime);
    _uiManager->update(deltaTime);
    renderUI();
}

void SettingsState::renderUI() {
    _background->render();
    _uiManager->render();

    auto window = _resourceManager->get<gfx::IWindow>();
    if (window) {
        std::vector<gfx::color_t> spectrumColors = {
            {255, 0, 0, 255},
            {255, 165, 0, 255},
            {255, 255, 0, 255},
            {0, 255, 0, 255},
            {0, 255, 255, 255},
            {0, 0, 255, 255},
            {128, 0, 128, 255}
        };

        size_t barWidth = static_cast<size_t>(constants::WINDOW_WIDTH) / spectrumColors.size();
        size_t barHeight = 50;
        size_t yPos = static_cast<size_t>(constants::WINDOW_HEIGHT) - barHeight;

        for (size_t i = 0; i < spectrumColors.size(); ++i) {
            size_t xPos = i * barWidth;
            window->drawFilledRectangle(
                spectrumColors[i], {xPos, yPos}, {barWidth, barHeight});
        }
    }
}

void SettingsState::cycleColorBlindnessFilter() {
    auto config = _resourceManager->get<SettingsConfig>();
    int currentState = config->getColorBlindnessState();
    int newState = (currentState + 1) % 4;
    config->setColorBlindnessState(newState);
    applyColorBlindnessFilter(newState);
    _colorBlindnessButton->setText(getColorBlindnessText(newState));
    _settingsManager->saveAccessibility();
}

void SettingsState::applyColorBlindnessFilter(int state) {
    if (!_resourceManager->has<gfx::IWindow>()) return;
    auto window = _resourceManager->get<gfx::IWindow>();
    if (!window) return;

    window->removeShaderFilter(constants::FILTER_PROTANOPIA_SHADER_PATH);
    window->removeShaderFilter(constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    window->removeShaderFilter(constants::FILTER_TRITANOPIA_SHADER_PATH);

    if (state == 1) {
        window->addShaderFilter(constants::FILTER_PROTANOPIA_SHADER_PATH);
    } else if (state == 2) {
        window->addShaderFilter(constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    } else if (state == 3) {
        window->addShaderFilter(constants::FILTER_TRITANOPIA_SHADER_PATH);
    }
}

void SettingsState::toggleHighContrastFilter() {
    auto config = _resourceManager->get<SettingsConfig>();
    bool newState = !config->isHighContrastEnabled();
    config->setHighContrastEnabled(newState);
    applyHighContrastFilter(newState);
    _settingsManager->saveAccessibility();
}

void SettingsState::applyHighContrastFilter(bool enabled) {
    if (!_resourceManager->has<gfx::IWindow>()) return;
    auto window = _resourceManager->get<gfx::IWindow>();
    if (!window) return;

    if (enabled) {
        window->addShaderFilter(constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setText("High Contrast: ON");
    } else {
        window->removeShaderFilter(constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setText("High Contrast: OFF");
    }
}

void SettingsState::updateBrightnessFilter(float value) {
    if (!_resourceManager->has<gfx::IWindow>()) return;
    auto window = _resourceManager->get<gfx::IWindow>();
    if (!window) return;

    value /= 100.0f;
    auto config = _resourceManager->get<SettingsConfig>();
    config->setBrightnessValue(value);

    window->addShaderFilter(constants::FILTER_BRIGHTNESS_SHADER_PATH);
    window->setShaderUniform(constants::FILTER_BRIGHTNESS_SHADER_PATH,
        constants::FILTER_BRIGHTNESS_UNIFORM_NAME, value);
    _settingsManager->saveAccessibility();
}

void SettingsState::cycleUIScale() {
    _uiManager->cycleGlobalScale();

    auto config = _resourceManager->get<SettingsConfig>();
    config->setUIScale(_uiManager->getGlobalScale());

    _scaleButton->setText(getUIScaleText(config->getUIScale()));
    _settingsManager->saveSettings();
}

void SettingsState::updateMusicVolume(float value) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setMusicVolume(value);

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->setMusicVolume(value);
    }
    _settingsManager->saveSettings();
}

void SettingsState::updateSoundVolume(float value) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setSoundVolume(value);

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->setSoundVolume(value);
    }
    _settingsManager->saveSettings();
}

void SettingsState::updateToggleValue(bool value) {
    auto inputProvider = std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(
        _resourceManager->get<ecs::IInputProvider>());
    if (inputProvider) {
        inputProvider->setToggleMode(value);
    }
    _settingsManager->saveKeybinds();
}

std::string SettingsState::getColorBlindnessText(int state) {
    switch (state) {
        case 1: return "Color Blindness: Protanopia";
        case 2: return "Color Blindness: Deuteranopia";
        case 3: return "Color Blindness: Tritanopia";
        default: return "Color Blindness: None";
    }
}

std::string SettingsState::getUIScaleText(ui::UIScale scale) {
    switch (scale) {
        case ui::UIScale::Small: return "UI Scale: Small";
        case ui::UIScale::Normal: return "UI Scale: Normal";
        case ui::UIScale::Large: return "UI Scale: Large";
        default: return "UI Scale: Normal";
    }
}

void SettingsState::setScreenResolution(SettingsConfig::ScreenResolution resolution) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setScreenResolution(resolution);
    _settingsManager->saveSettings();
    updateResolutionButtonColors(resolution);

    auto window = _resourceManager->get<gfx::IWindow>();
    if (config->isFullscreen(resolution)) {
        window->setFullscreen(true);
    } else {
        auto size = config->getScreenResolutionSize(resolution);
        window->resizeWindow(static_cast<size_t>(size.first),
            static_cast<size_t>(size.second));
    }
}

void SettingsState::updateResolutionButtonColors(SettingsConfig::ScreenResolution current) {
    std::vector<SettingsConfig::ScreenResolution> resolutions = {
        SettingsConfig::ScreenResolution::FULLSCREEN,
        SettingsConfig::ScreenResolution::RES_1920x1080,
        SettingsConfig::ScreenResolution::RES_1280x720,
        SettingsConfig::ScreenResolution::RES_1024x768,
        SettingsConfig::ScreenResolution::RES_800x600
    };

    for (size_t i = 0; i < _resolutionButtons.size(); ++i) {
        if (resolutions[i] == current) {
            _resolutionButtons[i]->setNormalColor(colors::BUTTON_SECONDARY);
            _resolutionButtons[i]->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
            _resolutionButtons[i]->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
            _resolutionButtons[i]->setState(ui::UIState::Disabled);
        } else {
            _resolutionButtons[i]->setNormalColor(colors::BUTTON_PRIMARY);
            _resolutionButtons[i]->setHoveredColor(colors::BUTTON_PRIMARY_HOVER);
            _resolutionButtons[i]->setPressedColor(colors::BUTTON_PRIMARY_PRESSED);
            _resolutionButtons[i]->setState(ui::UIState::Normal);
        }
    }
}

void SettingsState::updateTargetFPS(int fps) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setTargetFPS(fps);
    _settingsManager->saveSettings();

    auto window = _resourceManager->get<gfx::IWindow>();
    window->setFramerateLimit(static_cast<unsigned int>(fps));
}

void SettingsState::updateRenderQuality(float quality) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setRenderQuality(quality);

    auto window = _resourceManager->get<gfx::IWindow>();
    window->setRenderQuality(quality);

    _settingsManager->saveSettings();
}

void SettingsState::updateInGameMetrics(bool enabled) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setInGameMetricsEnabled(enabled);
    _settingsManager->saveSettings();
}

std::string SettingsState::getScreenResolutionText(
    SettingsConfig::ScreenResolution resolution
) {
    SettingsConfig tempConfig;
    return tempConfig.getScreenResolutionName(resolution);
}

void SettingsState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setViewCenter(_savedViewCenter.getX(), _savedViewCenter.getY());
    window->setCursor(false);
    _uiManager->clearElements();
    _backButton.reset();
    _highContrastButton.reset();
    _colorBlindnessButton.reset();
    _brightnessSlider.reset();
    _musicVolumeSlider.reset();
    _soundVolumeSlider.reset();
    _scaleButton.reset();
    _toggleSwitch.reset();
    _toggleLabel.reset();
    _toggleLayout.reset();
    for (auto& button : _resolutionButtons) {
        button.reset();
    }
    _resolutionButtons.clear();
    _fpsSlider.reset();
    _renderQualitySlider.reset();
    _inGameMetricsToggle.reset();
    _inGameMetricsLabel.reset();

    _moveUpLayout.reset();
    _moveUpLabel.reset();
    _moveUpPrimaryButton.reset();
    _moveUpSecondaryButton.reset();

    _moveDownLayout.reset();
    _moveDownLabel.reset();
    _moveDownPrimaryButton.reset();
    _moveDownSecondaryButton.reset();

    _moveLeftLayout.reset();
    _moveLeftLabel.reset();
    _moveLeftPrimaryButton.reset();
    _moveLeftSecondaryButton.reset();

    _moveRightLayout.reset();
    _moveRightLabel.reset();
    _moveRightPrimaryButton.reset();
    _moveRightSecondaryButton.reset();

    _shootLayout.reset();
    _shootLabel.reset();
    _shootPrimaryButton.reset();
    _shootSecondaryButton.reset();

    _centerColumnLayout.reset();

    _leftColumnLayout.reset();
    _rightColumnLayout.reset();
    _settingsLayout.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

void SettingsState::startKeyRebind(
    ecs::RemappableAction action, bool rebindPrimary,
    std::shared_ptr<ui::Button> button
) {
    _isWaitingForKey = true;
    _actionToRebind = action;
    _rebindingPrimary = rebindPrimary;
    _buttonToUpdate = button;

    std::string currentText = button->getText();
    size_t colonPos = currentText.find(':');
    if (colonPos != std::string::npos) {
        _rebindLabel = currentText.substr(0, colonPos);
    }

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        _originalKey = mappingManager->getKeyForRemappableAction(action, rebindPrimary);
    }

    std::string keyType = rebindPrimary ? "1" : "2";
    button->setText(keyType + ": Press key...");
}

void SettingsState::handleKeyRebind(gfx::EventType newKey) {
    if (!_actionToRebind.has_value() || !_buttonToUpdate)
        return;

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        mappingManager->remapKey(_actionToRebind.value(), newKey, _rebindingPrimary);
    }

    updateKeyBindingButtonText(_buttonToUpdate, _actionToRebind.value(), _rebindingPrimary);
    _settingsManager->saveKeybinds();

    _isWaitingForKey = false;
    _actionToRebind.reset();
    _buttonToUpdate.reset();
    _originalKey = gfx::EventType::NOTHING;
    _rebindLabel.clear();
}void SettingsState::updateKeyBindingButtonText(
    std::shared_ptr<ui::Button> button,
    ecs::RemappableAction action,
    bool isPrimary
) {
    std::string keyName = "";

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        gfx::EventType key = mappingManager->getKeyForRemappableAction(action, isPrimary);
        if (key != gfx::EventType::NOTHING) {
            keyName = ecs::InputMappingManager::eventTypeToString(key);
        }
    }

    std::string displayText = isPrimary ? "1" : "2";
    if (!keyName.empty()) {
        displayText += ": " + keyName;
    } else {
        displayText += ": None";
    }
    button->setText(displayText);
}

std::string SettingsState::getRemappableActionName(ecs::RemappableAction action) const {
    switch (action) {
        case ecs::RemappableAction::MOVE_LEFT: return "Move Left";
        case ecs::RemappableAction::MOVE_RIGHT: return "Move Right";
        case ecs::RemappableAction::MOVE_UP: return "Move Up";
        case ecs::RemappableAction::MOVE_DOWN: return "Move Down";
        case ecs::RemappableAction::SHOOT: return "Shoot";
        case ecs::RemappableAction::FORCE: return "Force";
        default: return "Unknown";
    }
}

}  // namespace gsm
