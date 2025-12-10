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

void SettingsConfig::saveAccessibility(const std::string& filepath) {
    nlohmann::json j;
    j["colorBlindnessState"] = _colorBlindnessState;
    j["brightnessValue"] = _brightnessValue;
    j["highContrastEnabled"] = _highContrastEnabled;
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
    if (j.contains("colorBlindnessState")) _colorBlindnessState = j["colorBlindnessState"];
    if (j.contains("brightnessValue")) _brightnessValue = j["brightnessValue"];
    if (j.contains("highContrastEnabled")) _highContrastEnabled = j["highContrastEnabled"];
}

void SettingsConfig::saveSettings(const std::string& filepath) {
    nlohmann::json j;
    j["uiScale"] = static_cast<int>(_uiScale);
    j["musicVolume"] = _musicVolume;
    j["soundVolume"] = _soundVolume;
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
    if (j.contains("uiScale")) _uiScale = static_cast<ui::UIScale>(j["uiScale"]);
    if (j.contains("musicVolume")) _musicVolume = j["musicVolume"];
    if (j.contains("soundVolume")) _soundVolume = j["soundVolume"];
}
