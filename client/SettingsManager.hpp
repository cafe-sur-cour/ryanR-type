/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SettingsManager
*/

#ifndef SETTINGSMANAGER_HPP_
#define SETTINGSMANAGER_HPP_

#include <memory>
#include "../../common/InputMapping/InputMappingManager.hpp"
#include "../../common/InputMapping/IInputProvider.hpp"
#include "SettingsConfig.hpp"
#include "../../common/interfaces/IWindow.hpp"

class SettingsManager {
public:
    SettingsManager(std::shared_ptr<ecs::InputMappingManager> mappingManager,
                    std::shared_ptr<ecs::IInputProvider> inputProvider,
                    std::shared_ptr<SettingsConfig> settingsConfig);
    ~SettingsManager() = default;

    void loadAll();
    void saveAll();

    void saveKeybinds();
    void loadKeybinds();

    void saveAccessibility();
    void loadAccessibility();

    void saveSettings();
    void loadSettings();

    void applyAccessibilityToWindow(std::shared_ptr<gfx::IWindow> window);

private:
    std::shared_ptr<ecs::InputMappingManager> _mappingManager;
    std::shared_ptr<ecs::IInputProvider> _inputProvider;
    std::shared_ptr<SettingsConfig> _settingsConfig;
};

#endif /* !SETTINGSMANAGER_HPP_ */
