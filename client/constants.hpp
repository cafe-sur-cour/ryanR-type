/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client Constants
*/

#ifndef CLIENT_CONSTANTS_HPP_
#define CLIENT_CONSTANTS_HPP_

#include "../common/constants.hpp"

namespace constants {
    /* Timeout */
    constexpr int NETWORK_TIMEOUT = 5;

    /* Window */
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    /* UI */
    constexpr float INVALID_MOUSE_POSITION = -1.0f;
    constexpr float NAVIGATION_COOLDOWN_TIME = 0.2f;
    constexpr float UI_SCALE_SMALL = 0.75f;
    constexpr float UI_SCALE_NORMAL = 1.0f;
    constexpr float UI_SCALE_LARGE = 1.25f;
    constexpr size_t BUTTON_FONT_SIZE_BASE = 24;

    /* View */
    constexpr float VIEW_SMOOTHING_SPEED = 15.0f;

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };

    /* Accessibility Filters */
    const std::string FILTER_HIGH_CONTRAST_SHADER_PATH = "assets/shaders/highcontrast.frag";
    const std::string FILTER_PROTANOPIA_SHADER_PATH = "assets/shaders/protanopia.frag";
    const std::string FILTER_DEUTERANOPIA_SHADER_PATH = "assets/shaders/deuteranopia.frag";
    const std::string FILTER_TRITANOPIA_SHADER_PATH = "assets/shaders/tritanopia.frag";
    const std::string FILTER_BRIGHTNESS_SHADER_PATH = "assets/shaders/brightness.frag";

    /* Health Bar */
    constexpr float HEALTH_BAR_OFFSET_Y = -10.0f;
    constexpr float HEALTH_BAR_HEIGHT = 5.0f;
    constexpr float HEALTH_BAR_OUTLINE_THICKNESS = 2.0f;

    /* Settings Parsing Constants */
    namespace settings {
        const std::string COLOR_BLINDNESS_STATE = "colorBlindnessState";
        const std::string BRIGHTNESS_VALUE = "brightnessValue";
        const std::string HIGH_CONTRAST_ENABLED = "highContrastEnabled";
        const std::string UI_SCALE = "uiScale";
        const std::string MUSIC_VOLUME = "musicVolume";
        const std::string SOUND_VOLUME = "soundVolume";
        const std::string PRIMARY = "primary";
        const std::string SECONDARY = "secondary";
        const std::string TOGGLE_MODE = "toggle_mode";
    }

    /* Paths */
    namespace paths {
        const std::string SAVES_DIR = "saves";
        const std::string KEYBINDS = "saves/keybinds.json";
        const std::string ACCESSIBILITY = "saves/accessibility.json";
        const std::string SETTINGS = "saves/settings.json";
    }
}

#endif /* !CLIENT_CONSTANTS_HPP_ */
