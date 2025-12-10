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
    j[constants::settings::COLOR_BLINDNESS_STATE] = _colorBlindnessState;
    j[constants::settings::BRIGHTNESS_VALUE] = _brightnessValue;
    j[constants::settings::HIGH_CONTRAST_ENABLED] = _highContrastEnabled;
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
    if (j.contains(constants::settings::COLOR_BLINDNESS_STATE)) _colorBlindnessState =
        j[constants::settings::COLOR_BLINDNESS_STATE];
    if (j.contains(constants::settings::BRIGHTNESS_VALUE)) _brightnessValue =
        j[constants::settings::BRIGHTNESS_VALUE];
    if (j.contains(constants::settings::HIGH_CONTRAST_ENABLED)) _highContrastEnabled =
        j[constants::settings::HIGH_CONTRAST_ENABLED];
}

void SettingsConfig::saveSettings(const std::string& filepath) {
    nlohmann::json j;
    j[constants::settings::UI_SCALE] = static_cast<int>(_uiScale);
    j[constants::settings::MUSIC_VOLUME] = _musicVolume;
    j[constants::settings::SOUND_VOLUME] = _soundVolume;
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
    if (j.contains(constants::settings::UI_SCALE)) _uiScale =
        static_cast<ui::UIScale>(j[constants::settings::UI_SCALE]);
    if (j.contains(constants::settings::MUSIC_VOLUME)) _musicVolume =
        j[constants::settings::MUSIC_VOLUME];
    if (j.contains(constants::settings::SOUND_VOLUME)) _soundVolume =
        j[constants::settings::SOUND_VOLUME];
}
