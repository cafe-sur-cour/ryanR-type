/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsState
*/

#include "SettingsState.hpp"
#include <memory>
#include <string>
#include <vector>
#include "../../../../../libs/Multimedia/IWindow.hpp"
#include "../../../../../libs/Multimedia/SfmlWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../../common/InputMapping/InputMappingManager.hpp"
#include "../../../../initResourcesManager/GraphicalInputProvider.hpp"
#include "../../../../SettingsConfig.hpp"
#include "../../../../../libs/Multimedia/IAudio.hpp"

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

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    auto inputProvider = _resourceManager->get<ecs::IInputProvider>();

    // Main horizontal layout to contain two columns
    ui::LayoutConfig settingsConfig;
    settingsConfig.direction = ui::LayoutDirection::Horizontal;
    settingsConfig.alignment = ui::LayoutAlignment::Center;
    settingsConfig.spacing = 40.0f;
    settingsConfig.padding = math::Vector2f(0.0f, 0.0f);
    settingsConfig.anchorX = ui::AnchorX::Center;
    settingsConfig.anchorY = ui::AnchorY::Center;
    settingsConfig.offset = math::Vector2f(0.0f, 0.0f);

    _settingsLayout = std::make_shared<ui::UILayout>(resourceManager, settingsConfig);
    _settingsLayout->setSize(math::Vector2f(900.f, 600.f));

    ui::LayoutConfig columnConfig;
    columnConfig.direction = ui::LayoutDirection::Vertical;
    columnConfig.alignment = ui::LayoutAlignment::Center;
    columnConfig.spacing = 15.0f;
    columnConfig.padding = math::Vector2f(0.0f, 0.0f);

    _leftColumnLayout = std::make_shared<ui::UILayout>(resourceManager, columnConfig);
    _leftColumnLayout->setSize(math::Vector2f(420.f, 600.f));

    _rightColumnLayout = std::make_shared<ui::UILayout>(resourceManager, columnConfig);
    _rightColumnLayout->setSize(math::Vector2f(420.f, 600.f));

    _scaleButton = std::make_shared<ui::Button>(resourceManager);
    _scaleButton->setText(getUIScaleText(config->getUIScale()));
    _scaleButton->setSize(math::Vector2f(380.f, 55.f));
    _scaleButton->setNormalColor({150, 100, 200});
    _scaleButton->setHoveredColor({200, 150, 255});
    _scaleButton->setFocusedColor({255, 200, 255});
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
    _brightnessSlider->setValue(config->getBrightnessValue());
    _brightnessSlider->setStep(5.0f);
    _brightnessSlider->setSize(math::Vector2f(380.f, 55.f));
    _brightnessSlider->setTrackColor({80, 80, 80});
    _brightnessSlider->setFillColor({100, 150, 200});
    _brightnessSlider->setHandleColor({150, 150, 150});
    _brightnessSlider->setHandleHoveredColor({200, 200, 200});
    _brightnessSlider->setHandleFocusedColor({255, 200, 100});
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
    _musicVolumeSlider->setTrackColor({80, 80, 80});
    _musicVolumeSlider->setFillColor({100, 200, 100});
    _musicVolumeSlider->setHandleColor({150, 150, 150});
    _musicVolumeSlider->setHandleHoveredColor({200, 200, 200});
    _musicVolumeSlider->setHandleFocusedColor({255, 200, 100});
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
    _soundVolumeSlider->setTrackColor({80, 80, 80});
    _soundVolumeSlider->setFillColor({200, 100, 100});
    _soundVolumeSlider->setHandleColor({150, 150, 150});
    _soundVolumeSlider->setHandleHoveredColor({200, 200, 200});
    _soundVolumeSlider->setHandleFocusedColor({255, 200, 100});
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
    _toggleSwitch->setTrackColor({80, 80, 80});
    _toggleSwitch->setHandleColor({150, 150, 150});
    _toggleSwitch->setHandleHoveredColor({200, 200, 200});
    _toggleSwitch->setHandleFocusedColor({255, 200, 100});
    _toggleSwitch->setOnColor({0, 200, 0});
    _toggleSwitch->setOffColor({200, 0, 0});
    _toggleSwitch->setOnValueChanged([this](bool value) {
        updateToggleValue(value);
    });

    _colorBlindnessButton = std::make_shared<ui::Button>(resourceManager);
    _colorBlindnessButton->setText(getColorBlindnessText(config->getColorBlindnessState()));
    _colorBlindnessButton->setSize(math::Vector2f(380.f, 55.f));
    _colorBlindnessButton->setNormalColor({100, 100, 150});
    _colorBlindnessButton->setHoveredColor({150, 150, 200});
    _colorBlindnessButton->setFocusedColor({100, 200, 255});
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
    _highContrastButton->setNormalColor(
        config->isHighContrastEnabled() ? gfx::color_t{0, 200, 0} : gfx::color_t{200, 0, 0}
    );
    _highContrastButton->setHoveredColor({150, 150, 200});
    _highContrastButton->setFocusedColor({100, 200, 255});
    _highContrastButton->setOnRelease([this]() {
        toggleHighContrastFilter();
    });
    _highContrastButton->setOnActivated([this]() {
        toggleHighContrastFilter();
    });

    _backButton = std::make_shared<ui::Button>(resourceManager);
    _backButton->setText("Back");
    _backButton->setSize(math::Vector2f(380.f, 55.f));
    _backButton->setNormalColor({200, 100, 0});
    _backButton->setHoveredColor({255, 150, 50});
    _backButton->setFocusedColor({255, 200, 100});
    _backButton->setOnRelease([this]() {
        this->_gsm->requestStatePop();
    });
    _backButton->setOnActivated([this]() {
        this->_gsm->requestStatePop();
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
    _moveUpLabel->setTextColor({255, 255, 255});
    _moveUpLabel->setFontSize(20);
    _moveUpLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveUpButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveUpButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveUpPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveUpPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveUpPrimaryButton->setNormalColor({80, 120, 160});
    _moveUpPrimaryButton->setHoveredColor({100, 150, 200});
    _moveUpPrimaryButton->setFocusedColor({120, 180, 240});
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
    _moveUpSecondaryButton->setNormalColor({60, 100, 140});
    _moveUpSecondaryButton->setHoveredColor({80, 120, 180});
    _moveUpSecondaryButton->setFocusedColor({100, 140, 220});
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
    _moveDownLabel->setTextColor({255, 255, 255});
    _moveDownLabel->setFontSize(20);
    _moveDownLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveDownButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveDownButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveDownPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveDownPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveDownPrimaryButton->setNormalColor({80, 120, 160});
    _moveDownPrimaryButton->setHoveredColor({100, 150, 200});
    _moveDownPrimaryButton->setFocusedColor({120, 180, 240});
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
    _moveDownSecondaryButton->setNormalColor({60, 100, 140});
    _moveDownSecondaryButton->setHoveredColor({80, 120, 180});
    _moveDownSecondaryButton->setFocusedColor({100, 140, 220});
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
    _moveLeftLabel->setTextColor({255, 255, 255});
    _moveLeftLabel->setFontSize(20);
    _moveLeftLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveLeftButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveLeftButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveLeftPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveLeftPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveLeftPrimaryButton->setNormalColor({80, 120, 160});
    _moveLeftPrimaryButton->setHoveredColor({100, 150, 200});
    _moveLeftPrimaryButton->setFocusedColor({120, 180, 240});
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
    _moveLeftSecondaryButton->setNormalColor({60, 100, 140});
    _moveLeftSecondaryButton->setHoveredColor({80, 120, 180});
    _moveLeftSecondaryButton->setFocusedColor({100, 140, 220});
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
    _moveRightLabel->setTextColor({255, 255, 255});
    _moveRightLabel->setFontSize(20);
    _moveRightLabel->setSize(math::Vector2f(380.f, 30.f));

    auto moveRightButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    moveRightButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _moveRightPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _moveRightPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _moveRightPrimaryButton->setNormalColor({80, 120, 160});
    _moveRightPrimaryButton->setHoveredColor({100, 150, 200});
    _moveRightPrimaryButton->setFocusedColor({120, 180, 240});
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
    _moveRightSecondaryButton->setNormalColor({60, 100, 140});
    _moveRightSecondaryButton->setHoveredColor({80, 120, 180});
    _moveRightSecondaryButton->setFocusedColor({100, 140, 220});
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
    _shootLabel->setTextColor({255, 255, 255});
    _shootLabel->setFontSize(20);
    _shootLabel->setSize(math::Vector2f(380.f, 30.f));

    auto shootButtonsLayout =
        std::make_shared<ui::UILayout>(resourceManager, horizontalConfig);
    shootButtonsLayout->setSize(math::Vector2f(380.f, 55.f));

    _shootPrimaryButton = std::make_shared<ui::Button>(resourceManager);
    _shootPrimaryButton->setSize(math::Vector2f(180.f, 50.f));
    _shootPrimaryButton->setNormalColor({80, 120, 160});
    _shootPrimaryButton->setHoveredColor({100, 150, 200});
    _shootPrimaryButton->setFocusedColor({120, 180, 240});
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
    _shootSecondaryButton->setNormalColor({60, 100, 140});
    _shootSecondaryButton->setHoveredColor({80, 120, 180});
    _shootSecondaryButton->setFocusedColor({100, 140, 220});
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
    _rightColumnLayout->addElement(_toggleSwitch);

    _settingsLayout->addElement(_leftColumnLayout);
    _settingsLayout->addElement(_rightColumnLayout);
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
            this->_gsm->requestStatePop();
        }
    });
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

    _uiManager->update(deltaTime);
    renderUI();
}

void SettingsState::renderUI() {
    _uiManager->render();

    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (sfmlWindow) {
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
            sfmlWindow->drawFilledRectangle(
                spectrumColors[i], {xPos, yPos}, {barWidth, barHeight});
        }
    }
}

void SettingsState::cycleColorBlindnessFilter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    auto config = _resourceManager->get<SettingsConfig>();
    int currentState = config->getColorBlindnessState();

    if (currentState == 1) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_PROTANOPIA_SHADER_PATH);
    } else if (currentState == 2) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    } else if (currentState == 3) {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_TRITANOPIA_SHADER_PATH);
    }

    int newState = (currentState + 1) % 4;
    config->setColorBlindnessState(newState);

    if (newState == 1) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_PROTANOPIA_SHADER_PATH);
    } else if (newState == 2) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    } else if (newState == 3) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_TRITANOPIA_SHADER_PATH);
    }
    _colorBlindnessButton->setText(getColorBlindnessText(newState));
    _settingsManager->saveAccessibility();
}

void SettingsState::toggleHighContrastFilter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    auto config = _resourceManager->get<SettingsConfig>();
    bool newState = !config->isHighContrastEnabled();
    config->setHighContrastEnabled(newState);

    if (newState) {
        sfmlWindow->getShaderManager().addFilter(
            constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setNormalColor({0, 200, 0});
        _highContrastButton->setText("High Contrast: ON");
    } else {
        sfmlWindow->getShaderManager().removeFilter(
            constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
        _highContrastButton->setNormalColor({200, 0, 0});
        _highContrastButton->setText("High Contrast: OFF");
    }
    _settingsManager->saveAccessibility();
}

void SettingsState::updateBrightnessFilter(float value) {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(window);
    if (!sfmlWindow) return;

    value /= 100.0f;
    auto config = _resourceManager->get<SettingsConfig>();
    config->setBrightnessValue(value);

    sfmlWindow->getShaderManager().addFilter(constants::FILTER_BRIGHTNESS_SHADER_PATH);
    sfmlWindow->getShaderManager().setUniform(
        constants::FILTER_BRIGHTNESS_SHADER_PATH, "brightness", value);
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

void SettingsState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setViewCenter(_savedViewCenter.getX(), _savedViewCenter.getY());
    _uiManager->clearElements();
    _backButton.reset();
    _highContrastButton.reset();
    _colorBlindnessButton.reset();
    _brightnessSlider.reset();
    _musicVolumeSlider.reset();
    _soundVolumeSlider.reset();
    _scaleButton.reset();
    _toggleSwitch.reset();

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
        displayText += ": Unassigned";
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
        default: return "Unknown";
    }
}

}  // namespace gsm
