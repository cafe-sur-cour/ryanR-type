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
#include "../../../../SettingsConfig.hpp"
#include "../../../../../libs/Multimedia/IAudio.hpp"

namespace gsm {

SettingsState::SettingsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
    
    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }
    
    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

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

    _moveUpKeyButton = std::make_shared<ui::Button>(resourceManager);
    _moveUpKeyButton->setSize(math::Vector2f(380.f, 55.f));
    _moveUpKeyButton->setNormalColor({80, 120, 160});
    _moveUpKeyButton->setHoveredColor({100, 150, 200});
    _moveUpKeyButton->setFocusedColor({120, 180, 240});
    updateKeyBindingButtonText(_moveUpKeyButton, ecs::InputAction::MOVE_Y, -1.0f, "Move Up");
    _moveUpKeyButton->setOnRelease([this]() {
        startKeyRebind(ecs::InputAction::MOVE_Y, -1.0f, _moveUpKeyButton);
    });
    _moveUpKeyButton->setOnActivated([this]() {
        startKeyRebind(ecs::InputAction::MOVE_Y, -1.0f, _moveUpKeyButton);
    });

    _moveDownKeyButton = std::make_shared<ui::Button>(resourceManager);
    _moveDownKeyButton->setSize(math::Vector2f(380.f, 55.f));
    _moveDownKeyButton->setNormalColor({80, 120, 160});
    _moveDownKeyButton->setHoveredColor({100, 150, 200});
    _moveDownKeyButton->setFocusedColor({120, 180, 240});
    updateKeyBindingButtonText(_moveDownKeyButton, ecs::InputAction::MOVE_Y, 1.0f, "Move Down");
    _moveDownKeyButton->setOnRelease([this]() {
        startKeyRebind(ecs::InputAction::MOVE_Y, 1.0f, _moveDownKeyButton);
    });
    _moveDownKeyButton->setOnActivated([this]() {
        startKeyRebind(ecs::InputAction::MOVE_Y, 1.0f, _moveDownKeyButton);
    });

    _moveLeftKeyButton = std::make_shared<ui::Button>(resourceManager);
    _moveLeftKeyButton->setSize(math::Vector2f(380.f, 55.f));
    _moveLeftKeyButton->setNormalColor({80, 120, 160});
    _moveLeftKeyButton->setHoveredColor({100, 150, 200});
    _moveLeftKeyButton->setFocusedColor({120, 180, 240});
    updateKeyBindingButtonText(_moveLeftKeyButton, ecs::InputAction::MOVE_X, -1.0f, "Move Left");
    _moveLeftKeyButton->setOnRelease([this]() {
        startKeyRebind(ecs::InputAction::MOVE_X, -1.0f, _moveLeftKeyButton);
    });
    _moveLeftKeyButton->setOnActivated([this]() {
        startKeyRebind(ecs::InputAction::MOVE_X, -1.0f, _moveLeftKeyButton);
    });

    _moveRightKeyButton = std::make_shared<ui::Button>(resourceManager);
    _moveRightKeyButton->setSize(math::Vector2f(380.f, 55.f));
    _moveRightKeyButton->setNormalColor({80, 120, 160});
    _moveRightKeyButton->setHoveredColor({100, 150, 200});
    _moveRightKeyButton->setFocusedColor({120, 180, 240});
    updateKeyBindingButtonText(_moveRightKeyButton, ecs::InputAction::MOVE_X, 1.0f, "Move Right");
    _moveRightKeyButton->setOnRelease([this]() {
        startKeyRebind(ecs::InputAction::MOVE_X, 1.0f, _moveRightKeyButton);
    });
    _moveRightKeyButton->setOnActivated([this]() {
        startKeyRebind(ecs::InputAction::MOVE_X, 1.0f, _moveRightKeyButton);
    });

    _shootKeyButton = std::make_shared<ui::Button>(resourceManager);
    _shootKeyButton->setSize(math::Vector2f(380.f, 55.f));
    _shootKeyButton->setNormalColor({80, 120, 160});
    _shootKeyButton->setHoveredColor({100, 150, 200});
    _shootKeyButton->setFocusedColor({120, 180, 240});
    updateKeyBindingButtonText(_shootKeyButton, ecs::InputAction::SHOOT, 1.0f, "Shoot");
    _shootKeyButton->setOnRelease([this]() {
        startKeyRebind(ecs::InputAction::SHOOT, 1.0f, _shootKeyButton);
    });
    _shootKeyButton->setOnActivated([this]() {
        startKeyRebind(ecs::InputAction::SHOOT, 1.0f, _shootKeyButton);
    });

    _leftColumnLayout->addElement(_musicVolumeSlider);
    _leftColumnLayout->addElement(_soundVolumeSlider);
    _leftColumnLayout->addElement(_brightnessSlider);
    _leftColumnLayout->addElement(_scaleButton);
    _leftColumnLayout->addElement(_colorBlindnessButton);
    _leftColumnLayout->addElement(_highContrastButton);
    _leftColumnLayout->addElement(_backButton);

    _rightColumnLayout->addElement(_moveUpKeyButton);
    _rightColumnLayout->addElement(_moveDownKeyButton);
    _rightColumnLayout->addElement(_moveLeftKeyButton);
    _rightColumnLayout->addElement(_moveRightKeyButton);
    _rightColumnLayout->addElement(_shootKeyButton);

    _settingsLayout->addElement(_leftColumnLayout);
    _settingsLayout->addElement(_rightColumnLayout);
    _uiManager->addElement(_settingsLayout);
}

void SettingsState::enter() {
    auto window = _resourceManager->get<gfx::IWindow>();
    _savedViewCenter = window->getViewCenter();
    auto logicalSize = window->getLogicalSize();
    window->setViewCenter(static_cast<float>(logicalSize.first) / 2.0f,
                          static_cast<float>(logicalSize.second) / 2.0f);
}

void SettingsState::update(float deltaTime) {
    (void)deltaTime;

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    if (_isWaitingForKey) {
        if (eventResult == gfx::EventType::ESCAPE) {
            _isWaitingForKey = false;
            if (_buttonToUpdate && _actionToRebind.has_value()) {
                updateKeyBindingButtonText(
                    _buttonToUpdate,
                    _actionToRebind.value(), 
                    _rebindDirection,
                    _rebindLabel
                );
            }
            _actionToRebind.reset();
            _buttonToUpdate.reset();
            _rebindLabel.clear();
        } else if (eventResult != gfx::EventType::NOTHING &&
            ecs::InputMappingManager::isKeyboardKey(eventResult)) {
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
}

void SettingsState::cycleUIScale() {
    _uiManager->cycleGlobalScale();

    auto config = _resourceManager->get<SettingsConfig>();
    config->setUIScale(_uiManager->getGlobalScale());

    _scaleButton->setText(getUIScaleText(config->getUIScale()));
}

void SettingsState::updateMusicVolume(float value) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setMusicVolume(value);

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->setMusicVolume(value);
    }
}

void SettingsState::updateSoundVolume(float value) {
    auto config = _resourceManager->get<SettingsConfig>();
    config->setSoundVolume(value);

    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->setSoundVolume(value);
    }
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
    _moveUpKeyButton.reset();
    _moveDownKeyButton.reset();
    _moveLeftKeyButton.reset();
    _moveRightKeyButton.reset();
    _shootKeyButton.reset();
    _leftColumnLayout.reset();
    _rightColumnLayout.reset();
    _settingsLayout.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

void SettingsState::startKeyRebind(
    ecs::InputAction action, float direction, 
    std::shared_ptr<ui::Button> button
) {
    _isWaitingForKey = true;
    _actionToRebind = action;
    _rebindDirection = direction;
    _buttonToUpdate = button;

    std::string currentText = button->getText();
    size_t colonPos = currentText.find(':');
    if (colonPos != std::string::npos) {
        _rebindLabel = currentText.substr(0, colonPos);
    }

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        _originalKey = mappingManager->getKeyboardKeyForActionDirection(action, direction);
    }

    button->setText("Press a key...");
}

void SettingsState::handleKeyRebind(gfx::EventType newKey) {
    if (!_actionToRebind.has_value() || !_buttonToUpdate)
        return;

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        mappingManager->remapKeyboardKey(_actionToRebind.value(), _originalKey, newKey);
    }

    updateKeyBindingButtonText(_buttonToUpdate, _actionToRebind.value(), _rebindDirection, _rebindLabel);

    _isWaitingForKey = false;
    _actionToRebind.reset();
    _buttonToUpdate.reset();
    _originalKey = gfx::EventType::NOTHING;
    _rebindLabel.clear();
}

void SettingsState::updateKeyBindingButtonText(
    std::shared_ptr<ui::Button> button, 
    ecs::InputAction action, float direction,
    const std::string& label
) {
    std::string keyName = "None";

    if (_resourceManager->has<ecs::InputMappingManager>()) {
        auto mappingManager = _resourceManager->get<ecs::InputMappingManager>();
        gfx::EventType key = mappingManager->getKeyboardKeyForActionDirection(action, direction);
        if (key != gfx::EventType::NOTHING) {
            keyName = ecs::InputMappingManager::eventTypeToString(key);
        }
    }

    button->setText(label + ": " + keyName);
}

std::string SettingsState::getActionName(ecs::InputAction action) const {
    switch (action) {
        case ecs::InputAction::MOVE_X: return "Move Horizontal";
        case ecs::InputAction::MOVE_Y: return "Move Vertical";
        case ecs::InputAction::SHOOT: return "Shoot";
        case ecs::InputAction::PAUSE: return "Pause";
        case ecs::InputAction::MENU_UP: return "Menu Up";
        case ecs::InputAction::MENU_DOWN: return "Menu Down";
        case ecs::InputAction::MENU_LEFT: return "Menu Left";
        case ecs::InputAction::MENU_RIGHT: return "Menu Right";
        case ecs::InputAction::MENU_SELECT: return "Menu Select";
        case ecs::InputAction::MENU_BACK: return "Menu Back";
        default: return "Unknown";
    }
}

}  // namespace gsm
