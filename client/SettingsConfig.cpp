/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsConfig
*/

#include "SettingsConfig.hpp"
#include <fstream>
#include <filesystem>  // NOLINT(build/c++17)
#include <memory>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include "constants.hpp"

void SettingsConfig::saveAccessibility(const std::string& filepath) {
    nlohmann::json j;
    j[constants::ACCESSIBILITY_COLOR_BLINDNESS_STATE] = _colorBlindnessState;
    j[constants::ACCESSIBILITY_BRIGHTNESS_VALUE] = _brightnessValue;
    j[constants::ACCESSIBILITY_HIGH_CONTRAST_ENABLED] = _highContrastEnabled;
    std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
    std::ofstream file(filepath);
    file << j.dump(4);
}

void SettingsConfig::loadAccessibility(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        saveAccessibility(filepath);
        return;
    }
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    nlohmann::json j;
    file >> j;
    if (j.contains(constants::ACCESSIBILITY_COLOR_BLINDNESS_STATE)) _colorBlindnessState =
        j[constants::ACCESSIBILITY_COLOR_BLINDNESS_STATE];
    if (j.contains(constants::ACCESSIBILITY_BRIGHTNESS_VALUE)) _brightnessValue =
        j[constants::ACCESSIBILITY_BRIGHTNESS_VALUE];
    if (j.contains(constants::ACCESSIBILITY_HIGH_CONTRAST_ENABLED)) _highContrastEnabled =
        j[constants::ACCESSIBILITY_HIGH_CONTRAST_ENABLED];
}

void SettingsConfig::saveSettings(const std::string& filepath) {
    nlohmann::json j;
    j[constants::SETTINGS_UI_SCALE] = static_cast<int>(_uiScale);
    j[constants::SETTINGS_MUSIC_VOLUME] = _musicVolume;
    j[constants::SETTINGS_SOUND_VOLUME] = _soundVolume;
    j[constants::SETTINGS_SCREEN_RESOLUTION] = static_cast<int>(_screenResolution);
    j[constants::SETTINGS_TARGET_FPS] = _targetFPS;
    j[constants::SETTINGS_RENDER_QUALITY] = _renderQuality;
    j[constants::SETTINGS_USERNAME] = _username;
    j[constants::SETTINGS_IN_GAME_METRICS_ENABLED] = _inGameMetricsEnabled;
    std::filesystem::create_directories(std::filesystem::path(filepath).parent_path());
    std::ofstream file(filepath);
    file << j.dump(4);
}

void SettingsConfig::loadSettings(const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        saveSettings(filepath);
        return;
    }
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    nlohmann::json j;
    file >> j;
    if (j.contains(constants::SETTINGS_UI_SCALE)) _uiScale =
        static_cast<ui::UIScale>(j[constants::SETTINGS_UI_SCALE]);
    if (j.contains(constants::SETTINGS_MUSIC_VOLUME)) _musicVolume =
        j[constants::SETTINGS_MUSIC_VOLUME];
    if (j.contains(constants::SETTINGS_SOUND_VOLUME)) _soundVolume =
        j[constants::SETTINGS_SOUND_VOLUME];
    if (j.contains(constants::SETTINGS_SCREEN_RESOLUTION)) _screenResolution =
        static_cast<ScreenResolution>(j[constants::SETTINGS_SCREEN_RESOLUTION]);
    if (j.contains(constants::SETTINGS_TARGET_FPS)) _targetFPS =
        j[constants::SETTINGS_TARGET_FPS];
    if (j.contains(constants::SETTINGS_RENDER_QUALITY)) _renderQuality =
        j[constants::SETTINGS_RENDER_QUALITY];
    if (j.contains(constants::SETTINGS_USERNAME)) _username = j[constants::SETTINGS_USERNAME];
    if (j.contains(constants::SETTINGS_IN_GAME_METRICS_ENABLED)) _inGameMetricsEnabled =
        j[constants::SETTINGS_IN_GAME_METRICS_ENABLED];
}

std::string SettingsConfig::getScreenResolutionName(ScreenResolution resolution) const {
    switch (resolution) {
        case ScreenResolution::RES_800x600: return "800x600";
        case ScreenResolution::RES_1024x768: return "1024x768";
        case ScreenResolution::RES_1280x720: return "1280x720";
        case ScreenResolution::RES_1920x1080: return "1920x1080";
        case ScreenResolution::FULLSCREEN: return "FULLSCREEN";
        default: return "1280x720";
    }
}

std::pair<int, int> SettingsConfig::getScreenResolutionSize(
    ScreenResolution resolution
) const {
    switch (resolution) {
        case ScreenResolution::RES_800x600: return {800, 600};
        case ScreenResolution::RES_1024x768: return {1024, 768};
        case ScreenResolution::RES_1280x720: return {1280, 720};
        case ScreenResolution::RES_1920x1080: return {1920, 1080};
        case ScreenResolution::FULLSCREEN: return {1920, 1080};
        default: return {1280, 720};
    }
}

bool SettingsConfig::isFullscreen(ScreenResolution resolution) const {
    return resolution == ScreenResolution::FULLSCREEN;
}
