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

    enum class ScreenResolution {
        RES_800x600 = 0,
        RES_1024x768 = 1,
        RES_1280x720 = 2,
        RES_1920x1080 = 3,
        FULLSCREEN = 4
    };

    ScreenResolution getScreenResolution() const { return _screenResolution; }
    void setScreenResolution(ScreenResolution resolution) { _screenResolution = resolution; }

    int getTargetFPS() const { return _targetFPS; }
    void setTargetFPS(int fps) { _targetFPS = fps; }

    float getRenderQuality() const { return _renderQuality; }
    void setRenderQuality(float quality) { _renderQuality = quality; }

    bool isInGameMetricsEnabled() const { return _inGameMetricsEnabled; }
    void setInGameMetricsEnabled(bool enabled) { _inGameMetricsEnabled = enabled; }

    std::string getUsername() const { return _username; }
    void setUsername(const std::string& username) { _username = username; }

    std::string getScreenResolutionName(ScreenResolution resolution) const;
    std::pair<int, int> getScreenResolutionSize(ScreenResolution resolution) const;
    bool isFullscreen(ScreenResolution resolution) const;

    void saveAccessibility(const std::string& filepath = constants::ACCESSIBILITY_FILE_PATH);
    void loadAccessibility(const std::string& filepath = constants::ACCESSIBILITY_FILE_PATH);

    void saveSettings(const std::string& filepath = constants::SETTINGS_FILE_PATH);
    void loadSettings(const std::string& filepath = constants::SETTINGS_FILE_PATH);

private:
    int _colorBlindnessState = 0;
    float _brightnessValue = 1.0f;
    bool _highContrastEnabled = false;
    ui::UIScale _uiScale = ui::UIScale::Normal;
    float _musicVolume = 100.0f;
    float _soundVolume = 100.0f;
    ScreenResolution _screenResolution = ScreenResolution::RES_1920x1080;
    int _targetFPS = 60;
    float _renderQuality = 1.0f;
    bool _inGameMetricsEnabled = false;
    std::string _username = "";
};

#endif  // SETTINGSCONFIG_HPP_
