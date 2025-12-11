/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsManager
*/

#include "SettingsManager.hpp"
#include <fstream>
#include <filesystem>  // NOLINT(build/c++17)
#include <memory>
#include <nlohmann/json.hpp>
#include "initResourcesManager/GraphicalInputProvider.hpp"
#include "constants.hpp"

SettingsManager::SettingsManager(std::shared_ptr<ecs::InputMappingManager> mappingManager,
    std::shared_ptr<ecs::IInputProvider> inputProvider,
    std::shared_ptr<SettingsConfig> settingsConfig
) : _mappingManager(mappingManager),
_inputProvider(inputProvider),
_settingsConfig(settingsConfig) {
}

void SettingsManager::loadAll() {
    loadKeybinds();
    loadAccessibility();
    loadSettings();
}

void SettingsManager::saveAll() {
    saveKeybinds();
    saveAccessibility();
    saveSettings();
}

void SettingsManager::saveKeybinds() {
    nlohmann::json j;
    for (const auto& [action, binding] : _mappingManager->getMapping().remappableKeys) {
        j[ecs::InputMappingManager::remappableActionToString(action)] = {
            {constants::KEYBIND_PRIMARY,
                ecs::InputMappingManager::eventTypeToString(binding.primary)},
            {constants::KEYBIND_SECONDARY,
                ecs::InputMappingManager::eventTypeToString(binding.secondary)}
        };
    }
    auto graphicalProvider =
        std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
    if (graphicalProvider) {
        j[constants::KEYBIND_TOGGLE_MODE] = graphicalProvider->isToggleMode();
    }
    std::filesystem::create_directories(constants::SAVES_DIRECTORY);
    std::ofstream file(constants::KEYBINDS_FILE_PATH);
    file << j.dump(4);
}

void SettingsManager::loadKeybinds() {
    std::filesystem::path filepath(constants::KEYBINDS_FILE_PATH);
    if (!std::filesystem::exists(filepath)) {
        saveKeybinds();
        return;
    }
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    nlohmann::json j;
    file >> j;
    for (const auto& [key, value] : j.items()) {
        if (key == constants::KEYBIND_TOGGLE_MODE) {
            auto graphicalProvider =
                std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
            if (graphicalProvider && value.is_boolean()) {
                graphicalProvider->setToggleMode(value);
            }
        } else {
            ecs::RemappableAction action =
                ecs::InputMappingManager::stringToRemappableAction(key);
            if (value.contains(constants::KEYBIND_PRIMARY)) {
                _mappingManager->getMutableMapping().remappableKeys[action].primary =
                    ecs::InputMappingManager::stringToEventType(
                        value[constants::KEYBIND_PRIMARY]);
            }
            if (value.contains(constants::KEYBIND_SECONDARY)) {
                _mappingManager->getMutableMapping().remappableKeys[action].secondary =
                    ecs::InputMappingManager::stringToEventType(
                        value[constants::KEYBIND_SECONDARY]);
            }
        }
    }
}

void SettingsManager::saveAccessibility() {
    _settingsConfig->saveAccessibility();
}

void SettingsManager::loadAccessibility() {
    _settingsConfig->loadAccessibility();
}

void SettingsManager::saveSettings() {
    _settingsConfig->saveSettings();
}

void SettingsManager::loadSettings() {
    _settingsConfig->loadSettings();
}

void SettingsManager::applyAccessibilityToWindow(std::shared_ptr<gfx::IWindow> window) {
    int colorBlindnessState = _settingsConfig->getColorBlindnessState();
    window->removeShaderFilter(constants::FILTER_PROTANOPIA_SHADER_PATH);
    window->removeShaderFilter(constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    window->removeShaderFilter(constants::FILTER_TRITANOPIA_SHADER_PATH);
    if (colorBlindnessState == 1) {
        window->addShaderFilter(constants::FILTER_PROTANOPIA_SHADER_PATH);
    } else if (colorBlindnessState == 2) {
        window->addShaderFilter(constants::FILTER_DEUTERANOPIA_SHADER_PATH);
    } else if (colorBlindnessState == 3) {
        window->addShaderFilter(constants::FILTER_TRITANOPIA_SHADER_PATH);
    }

    if (_settingsConfig->isHighContrastEnabled()) {
        window->addShaderFilter(constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
    } else {
        window->removeShaderFilter(constants::FILTER_HIGH_CONTRAST_SHADER_PATH);
    }

    float brightness = _settingsConfig->getBrightnessValue();
    window->addShaderFilter(constants::FILTER_BRIGHTNESS_SHADER_PATH);
    window->setShaderUniform(constants::FILTER_BRIGHTNESS_SHADER_PATH,
        constants::FILTER_BRIGHTNESS_UNIFORM_NAME, brightness);
}
