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
            {constants::settings::PRIMARY,
                ecs::InputMappingManager::eventTypeToString(binding.primary)},
            {constants::settings::SECONDARY,
                ecs::InputMappingManager::eventTypeToString(binding.secondary)}
        };
    }
    auto graphicalProvider =
        std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
    if (graphicalProvider) {
        j[constants::settings::TOGGLE_MODE] = graphicalProvider->isToggleMode();
    }
    std::filesystem::create_directories(constants::paths::SAVES_DIR);
    std::ofstream file(constants::paths::KEYBINDS);
    file << j.dump(4);
}

void SettingsManager::loadKeybinds() {
    std::filesystem::path filepath(constants::paths::KEYBINDS);
    if (!std::filesystem::exists(filepath)) {
        saveKeybinds();
        return;
    }
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    nlohmann::json j;
    file >> j;
    for (const auto& [key, value] : j.items()) {
        if (key == constants::settings::TOGGLE_MODE) {
            auto graphicalProvider =
                std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
            if (graphicalProvider && value.is_boolean()) {
                graphicalProvider->setToggleMode(value);
            }
        } else {
            ecs::RemappableAction action =
                ecs::InputMappingManager::stringToRemappableAction(key);
            if (value.contains(constants::settings::PRIMARY)) {
                _mappingManager->getMutableMapping().remappableKeys[action].primary =
                    ecs::InputMappingManager::stringToEventType(
                        value[constants::settings::PRIMARY]);
            }
            if (value.contains(constants::settings::SECONDARY)) {
                _mappingManager->getMutableMapping().remappableKeys[action].secondary =
                    ecs::InputMappingManager::stringToEventType(
                        value[constants::settings::SECONDARY]);
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
