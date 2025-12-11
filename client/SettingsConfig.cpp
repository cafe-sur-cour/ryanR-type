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
}
