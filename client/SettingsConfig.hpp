/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsConfig
*/

#ifndef SETTINGSCONFIG_HPP_
#define SETTINGSCONFIG_HPP_

#include "ui/elements/base/UIElement.hpp"
#include "constants.hpp"

class SettingsConfig {
public:
    SettingsConfig() = default;
    ~SettingsConfig() = default;

    int getColorBlindnessState() const { return _colorBlindnessState; }
    void setColorBlindnessState(int state) { _colorBlindnessState = state; }

    float getBrightnessValue() const { return _brightnessValue; }
    void setBrightnessValue(float value) { _brightnessValue = value; }

    bool isHighContrastEnabled() const { return _highContrastEnabled; }
    void setHighContrastEnabled(bool enabled) { _highContrastEnabled = enabled; }

    ui::UIScale getUIScale() const { return _uiScale; }
    void setUIScale(ui::UIScale scale) { _uiScale = scale; }

    float getMusicVolume() const { return _musicVolume; }
    void setMusicVolume(float volume) { _musicVolume = volume; }

    float getSoundVolume() const { return _soundVolume; }
    void setSoundVolume(float volume) { _soundVolume = volume; }

    void saveAccessibility(const std::string& filepath = constants::ACCESSIBILITY_FILE_PATH);
    void loadAccessibility(const std::string& filepath = constants::ACCESSIBILITY_FILE_PATH);

    void saveSettings(const std::string& filepath = constants::SETTINGS_FILE_PATH);
    void loadSettings(const std::string& filepath = constants::SETTINGS_FILE_PATH);

private:
    int _colorBlindnessState = 0;
    float _brightnessValue = 100.0f;
    bool _highContrastEnabled = false;
    ui::UIScale _uiScale = ui::UIScale::Normal;
    float _musicVolume = 100.0f;
    float _soundVolume = 100.0f;
};

#endif  // SETTINGSCONFIG_HPP_
