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

    ui::LayoutConfig settingsConfig;
    settingsConfig.direction = ui::LayoutDirection::Vertical;
    settingsConfig.alignment = ui::LayoutAlignment::Center;
    settingsConfig.spacing = 20.0f;
    settingsConfig.padding = math::Vector2f(0.0f, 0.0f);
    settingsConfig.anchorX = ui::AnchorX::Center;
    settingsConfig.anchorY = ui::AnchorY::Center;
    settingsConfig.offset = math::Vector2f(0.0f, 0.0f);

    _settingsLayout = std::make_shared<ui::UILayout>(resourceManager, settingsConfig);
    _settingsLayout->setSize(math::Vector2f(576.f, 500.f));

    _scaleButton = std::make_shared<ui::Button>(resourceManager);
    _scaleButton->setText(getUIScaleText(config->getUIScale()));
    _scaleButton->setSize(math::Vector2f(400.f, 65.f));
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
    _brightnessSlider->setSize(math::Vector2f(400.f, 65.f));
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
    _musicVolumeSlider->setSize(math::Vector2f(400.f, 65.f));
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
    _soundVolumeSlider->setSize(math::Vector2f(400.f, 65.f));
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
    _colorBlindnessButton->setSize(math::Vector2f(400.f, 65.f));
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
    _highContrastButton->setSize(math::Vector2f(400.f, 65.f));
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
    _backButton->setSize(math::Vector2f(400.f, 65.f));
    _backButton->setNormalColor({200, 100, 0});
    _backButton->setHoveredColor({255, 150, 50});
    _backButton->setFocusedColor({255, 200, 100});
    _backButton->setOnRelease([this]() {
        this->_gsm->requestStatePop();
    });
    _backButton->setOnActivated([this]() {
        this->_gsm->requestStatePop();
    });

    _settingsLayout->addElement(_musicVolumeSlider);
    _settingsLayout->addElement(_soundVolumeSlider);
    _settingsLayout->addElement(_scaleButton);
    _settingsLayout->addElement(_brightnessSlider);
    _settingsLayout->addElement(_colorBlindnessButton);
    _settingsLayout->addElement(_highContrastButton);
    _settingsLayout->addElement(_backButton);
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
    _settingsLayout.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
