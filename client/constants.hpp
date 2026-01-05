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
    constexpr float NAVIGATION_COOLDOWN_TIME = 0.1f;
    constexpr float UI_SCALE_SMALL = 0.75f;
    constexpr float UI_SCALE_NORMAL = 1.0f;
    constexpr float UI_SCALE_LARGE = 1.25f;
    constexpr size_t BUTTON_FONT_SIZE_BASE = 24;
    constexpr double SMALL_FONT_SIZE_MULTIPLIER = 0.5;

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
    const std::string FILTER_BRIGHTNESS_UNIFORM_NAME = "brightness";

    /* Health Bar */
    constexpr float HEALTH_BAR_OFFSET_Y = -10.0f;
    constexpr float HEALTH_BAR_HEIGHT = 5.0f;
    constexpr float HEALTH_BAR_OUTLINE_THICKNESS = 2.0f;

    /* Settings Parsing Constants */
    const std::string ACCESSIBILITY_COLOR_BLINDNESS_STATE = "colorBlindnessState";
    const std::string ACCESSIBILITY_BRIGHTNESS_VALUE = "brightnessValue";
    const std::string ACCESSIBILITY_HIGH_CONTRAST_ENABLED = "highContrastEnabled";
    const std::string SETTINGS_UI_SCALE = "uiScale";
    const std::string SETTINGS_MUSIC_VOLUME = "musicVolume";
    const std::string SETTINGS_SOUND_VOLUME = "soundVolume";
    const std::string SETTINGS_SCREEN_RESOLUTION = "screenResolution";
    const std::string SETTINGS_TARGET_FPS = "targetFPS";
    const std::string SETTINGS_RENDER_QUALITY = "renderQuality";
    const std::string KEYBIND_PRIMARY = "primary";
    const std::string KEYBIND_SECONDARY = "secondary";
    const std::string KEYBIND_TOGGLE_MODE = "toggle_mode";

    /* Paths */
    const std::string SAVES_DIRECTORY = "saves";
    const std::string KEYBINDS_FILE_PATH = "saves/keybinds.json";
    const std::string ACCESSIBILITY_FILE_PATH = "saves/accessibility.json";
    const std::string SETTINGS_FILE_PATH = "saves/settings.json";
    const std::string UI_BACKGROUND_EARTH_PATH = "assets/ui/earth.jpg";
    const std::string HOW_TO_PLAY_PATH = "assets/sprites/how_to_play.png";

    const std::string WIN_TEXT = "YOU WIN!";
    const std::string LOSE_TEXT = "YOU LOSE!";

    /* Home page input place holders */
    const std::string IP_PLACEHOLDER = "Enter an IP address";
    const std::string PORT_PLACEHOLDER = "Enter a port";
}

#endif /* !CLIENT_CONSTANTS_HPP_ */
