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
            {"primary", ecs::InputMappingManager::eventTypeToString(binding.primary)},
            {"secondary", ecs::InputMappingManager::eventTypeToString(binding.secondary)}
        };
    }
    auto graphicalProvider =
        std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
    if (graphicalProvider) {
        j["toggle_mode"] = graphicalProvider->isToggleMode();
    }
    std::filesystem::create_directories("saves");
    std::ofstream file("saves/keybinds.json");
    file << j.dump(4);
}

void SettingsManager::loadKeybinds() {
    std::filesystem::path filepath("saves/keybinds.json");
    if (!std::filesystem::exists(filepath)) {
        saveKeybinds();
        return;
    }
    std::ifstream file(filepath);
    if (!file.is_open()) return;
    nlohmann::json j;
    file >> j;
    for (const auto& [key, value] : j.items()) {
        if (key == "toggle_mode") {
            auto graphicalProvider =
                std::dynamic_pointer_cast<ecs::GraphicalInputProvider>(_inputProvider);
            if (graphicalProvider && value.is_boolean()) {
                graphicalProvider->setToggleMode(value);
            }
        } else {
            ecs::RemappableAction action =
                ecs::InputMappingManager::stringToRemappableAction(key);
            if (value.contains("primary")) {
                _mappingManager->getMutableMapping().remappableKeys[action].primary =
                    ecs::InputMappingManager::stringToEventType(value["primary"]);
            }
            if (value.contains("secondary")) {
                _mappingManager->getMutableMapping().remappableKeys[action].secondary =
                    ecs::InputMappingManager::stringToEventType(value["secondary"]);
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
